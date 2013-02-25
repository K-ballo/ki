
#pragma warning( push )
#pragma warning( disable: 4146 ) // unary minus operator applied to unsigned type, result still unsigned
#pragma warning( disable: 4244 ) // 'return' : conversion from 'uint64_t' to 'unsigned int', possible loss of data
#pragma warning( disable: 4800 ) // 'unsigned int' : forcing value to bool 'true' or 'false' (performance warning)
#include <llvm/DerivedTypes.h>
#include <llvm/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Type.h>
#pragma warning( pop )

#include <boost/assert.hpp>

#include <boost/noncopyable.hpp>

#include <boost/variant.hpp>

#include <map>
#include <tuple>
#include <set>
#include <string>
#include <vector>

namespace ki {

    struct class_decl;
    struct class_sig;
    struct class_inst;
    
    struct class_sig
    {
        class_decl const* decl;
        std::vector< class_sig > args;

        /*implicit*/ class_sig(
            class_decl const* decl = nullptr
          , std::vector< class_sig > args = std::vector< class_sig >()
        ) : decl( decl )
          , args( args )
        {}

        friend bool operator <(
            class_sig const& left
          , class_sig const& right
        )
        {
            return
                left.decl < right.decl
             || ( left.decl == right.decl && left.args < right.args )
                ;
        }
    };

    class module
    {
    public:
        explicit module(
            std::string const& name
          , llvm::LLVMContext& context
        ) : _context( context )
          , _module( name, context )
        {}

        llvm::LLVMContext& context()
        {
            return _context;
        }

        llvm::Module* module_ptr()
        {
            return &_module;
        }
        
        class_decl const* get_class( std::string const& name ) const;
        class_inst const* instantiate( class_sig const& signature );

        class_decl* declare_class( std::string const& name );
        class_inst* define_class( class_inst const& instantiation );

    private:
        llvm::LLVMContext& _context;
        llvm::Module _module;

        mutable std::map< std::string, class_decl > _class_declarations;
        std::map< class_sig, class_inst > _class_instantiations;
    };

    // declarations
    struct class_decl
    {
        typedef
            std::pair< std::string, class_sig >
            member_type;

        std::string const name;
        std::vector< class_decl const* > params;

        std::vector< member_type > members;

        explicit class_decl( std::string const& name = "" )
          : name( name )
        {}
    };

    // instantiations
    struct class_inst
    {
        typedef
            std::map< class_decl const*, class_sig >
            param_map_type;

        class_sig const signature;
        param_map_type const params;

        llvm::Type* llvm_type;
        
        explicit class_inst( class_sig const& signature )
          : signature( signature )
          , params( make_params_map( signature.decl->params, signature.args ) )

          , llvm_type( nullptr )
        {}

        static param_map_type make_params_map(
            std::vector< class_decl const* > const& params
          , std::vector< class_sig > const& args
        )
        {
            BOOST_ASSERT(( params.size() == args.size() ));

            param_map_type params_map;
            auto param_iter = params.begin();
            for(
                auto
                    arg_iter = args.begin()
                  , arg_end = args.end()
              ; arg_iter != arg_end
              ; ++param_iter, ++arg_iter
            )
            {
                params_map.emplace( *param_iter, *arg_iter );
            }

            return params_map;
        }

        class_sig map_template_arguments( class_sig signature ) const
        {
            auto decl_iter = params.find( signature.decl );
            if( decl_iter != params.end() )
            {
                BOOST_ASSERT(( signature.args.empty() ));

                signature = decl_iter->second;
            } else {
                for( auto& signature_arg : signature.args )
                {
                    signature_arg = map_template_arguments( signature_arg );
                }
            }

            return signature;
        }
    };

} // namespace ki

/* module.cpp */

namespace ki {

    class_decl const* module::get_class( std::string const& name ) const
    {
        auto class_iter = _class_declarations.find( name );
        if( class_iter != _class_declarations.end() )
            return &class_iter->second;

        return
            &_class_declarations.emplace(
                std::piecewise_construct
              , std::forward_as_tuple( name )
              , std::forward_as_tuple( name )
            ).first->second;
    }

    class_inst const* module::instantiate( class_sig const& signature )
    {
        auto class_iter = _class_instantiations.find( signature );
        if( class_iter != _class_instantiations.end() )
            return &class_iter->second;
        
        llvm::StructType* llvm_type =
            llvm::StructType::create( _context, signature.decl->name );

        class_inst* instantiation =
            &_class_instantiations.emplace(
                std::piecewise_construct
              , std::forward_as_tuple( signature )
              , std::forward_as_tuple( signature )
            ).first->second;
        instantiation->llvm_type = llvm_type;

        std::vector< llvm::Type* > llvm_member_types;
        for( auto const& member : signature.decl->members )
        {
            class_sig const member_signature =
                instantiation->map_template_arguments( member.second );

            class_inst const* member_instantiation =
                instantiate( member_signature );

            llvm_member_types.push_back( member_instantiation->llvm_type );
        }
        llvm_type->setBody( llvm_member_types );

        return instantiation;
    }
    
    class_decl* module::declare_class( std::string const& name )
    {
        llvm::StructType::create( _context, name );

        return
            &_class_declarations.emplace(
                std::piecewise_construct
              , std::forward_as_tuple( name )
              , std::forward_as_tuple( name )
            ).first->second;
    }

    class_inst* module::define_class( class_inst const& instantiation )
    {
        _class_declarations.emplace(
            std::piecewise_construct
          , std::forward_as_tuple( instantiation.signature.decl->name )
          , std::forward_as_tuple( *instantiation.signature.decl )
        );

        return
            &_class_instantiations.emplace(
                std::piecewise_construct
              , std::forward_as_tuple( instantiation.signature )
              , std::forward_as_tuple( instantiation )
            ).first->second;
    }

} // namespace ki

#include <iostream>

std::map< std::string, ki::class_decl > classes;

//void resolve_function( std::string const& name )
//{
//    auto iter_lower = functions.lower_bound( name );
//    auto iter_upper = functions.upper_bound( name );
//
//    if( iter_lower != iter_upper )
//    {
//    } else {
//        std::cerr
//         << "'" << name << "' : No function found"
//         << std::endl;
//    }
//}
int main( int argc, char** argv )
{
    llvm::LLVMContext context;
    ki::module module( "wind", context );
    llvm::IRBuilder<> builder( module.context() );

    // lang classes
    {
        ki::class_decl* int_decl = module.declare_class( "int" );

        ki::class_inst int_inst(( ki::class_sig( int_decl ) ));
        int_inst.llvm_type = llvm::Type::getInt32Ty( module.context() );
        module.define_class( int_inst );
    }

    // some tests
    ki::class_decl* pair = module.declare_class( "pair" );
    pair->params.push_back( module.get_class( "pair::T" ) );
    pair->params.push_back( module.get_class( "pair::U" ) );
    pair->members.push_back( std::make_pair( "first", ki::class_sig( pair->params[0] ) ) );
    pair->members.push_back( std::make_pair( "second", ki::class_sig( pair->params[1] ) ) );

    std::vector< ki::class_sig > pair_args;
    pair_args.push_back( module.get_class( "int" ) );
    pair_args.push_back( module.get_class( "int" ) );
    ki::class_inst const* int_pair = module.instantiate( ki::class_sig( pair, pair_args ) );

    std::vector< ki::class_sig > tripair_args;
    tripair_args.push_back( module.get_class( "int" ) );
    tripair_args.push_back( int_pair->signature );
    ki::class_inst const* int_tripair = module.instantiate( ki::class_sig( pair, tripair_args ) );
    
    // add a main function
    {
        llvm::FunctionType* funcType = llvm::FunctionType::get( builder.getVoidTy(), false );
        llvm::Function* mainFunc = 
            llvm::Function::Create( funcType, llvm::Function::ExternalLinkage, "main", module.module_ptr() );
        llvm::BasicBlock* entry = llvm::BasicBlock::Create( module.context(), "entrypoint", mainFunc );
        builder.SetInsertPoint( entry );

        builder.CreateAlloca( int_pair->llvm_type, 0, "int_pair" );
        builder.CreateAlloca( int_tripair->llvm_type, 0, "int_tripair" );

        builder.CreateRetVoid();
    }
    
    // and output now
    module.module_ptr()->dump();

    return 0;
}
