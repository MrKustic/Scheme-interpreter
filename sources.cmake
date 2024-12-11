add_library(scheme_tidy
        lib/tokenizer.cpp
        lib/parser.cpp
        lib/scheme.cpp
        lib/object.cpp
        lib/operations_impl/cell_evaluation.cpp
        lib/operations_impl/list_operations.cpp
        lib/operations_impl/integer_operations.cpp
        lib/operations_impl/boolean_operations.cpp
        lib/operations_impl/variables.cpp
        lib/helpers.cpp
        lib/operations_impl/lambda.cpp
        lib/garbage_collector.cpp
)
