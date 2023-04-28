#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <sstream>
#include <cstring>
#include <fstream>
#include <clang-c/Index.h>

enum CXChildVisitResult getChildCursorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    CXCursor *child_cursor = reinterpret_cast<CXCursor*>(client_data);
    *child_cursor = cursor;
    return CXChildVisit_Break; // Stop visiting after the first child
}

std::string get_surrounding_context(CXSourceLocation location, int before, int after) {
    unsigned line_number;
    CXFile file;
    clang_getSpellingLocation(location, &file, &line_number, nullptr, nullptr);
    CXString file_name_cxstr = clang_getFileName(file);
    const char *file_name = clang_getCString(file_name_cxstr);
    std::ifstream file_stream(file_name);
    std::string line, context;
    for (unsigned i = 1; std::getline(file_stream, line); ++i) {
        if (i >= line_number - before && i <= line_number + after) {
            context += line + '\n';
        }
    }
    clang_disposeString(file_name_cxstr);
    return context;
}

// std::string get_function_name(CXCursor cursor) {
//     CXCursor parent =  clang_getCursorSemanticParent(cursor);
//     int test = 0;
//     while (clang_getCursorKind(parent) != CXCursor_FunctionDecl &&
//            clang_getCursorKind(parent) != CXCursor_CXXMethod &&
//            test < 100) {
//         CXString test_parent_name = clang_getCursorKindSpelling(clang_getCursorKind(parent));
//         std::cout << clang_getCString(test_parent_name) << std::endl;
//         parent = clang_getCursorSemanticParent(parent);
//         test ++;
//     }
//     CXString parent_name = clang_getCursorSpelling(parent);
//     std::string function_name(clang_getCString(parent_name));
//     clang_disposeString(parent_name);
//     return function_name;
// }

std::vector<std::pair<std::string, std::pair<std::string, std::string>>> variables_and_context_in_functions;

void print_ast(CXCursor cursor, int depth) {
    if (clang_getCursorKind(cursor) == CXCursor_FirstInvalid)
        return;

    for (int i = 0; i < depth; ++i)
        std::cout << "  ";

    CXString cursor_kind_name = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
    CXString cursor_spelling = clang_getCursorSpelling(cursor);
    std::cout << clang_getCString(cursor_kind_name) << " " << clang_getCString(cursor_spelling) << std::endl;

    if (clang_getCursorKind(cursor) == CXCursor_CallExpr && clang_getCString(cursor_spelling) == std::string("scanf")) {

        std::string surrounding_context = get_surrounding_context(clang_getCursorLocation(cursor), 1, 1);

        // std::string function_name = get_function_name(cursor);
        std::string function_name = "TODO";

        CXCursor arg_cursor = clang_Cursor_getArgument(cursor, 1);
        if (clang_getCursorKind(arg_cursor) == CXCursor_UnaryOperator) {
            clang_visitChildren(arg_cursor, getChildCursorVisitor, &arg_cursor);
        }
        CXString arg_spelling = clang_getCursorDisplayName(arg_cursor);
        CXType arg_type = clang_getPointeeType(clang_getCursorType(arg_cursor));
        
        std::string var_name(clang_getCString(arg_spelling));
        variables_and_context_in_functions.emplace_back(var_name, std::make_pair(function_name, surrounding_context));
    }
    
    clang_disposeString(cursor_kind_name);
    clang_disposeString(cursor_spelling);

    clang_visitChildren(cursor,
                        [](CXCursor c, CXCursor parent, CXClientData client_data) {
                            int *depth = reinterpret_cast<int*>(client_data);
                            print_ast(c, *depth + 1);
                            return CXChildVisit_Continue;
                        },
                        &depth);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ast_printer <path_to_c_file>" << std::endl;
        return 1;
    }

    CXIndex index = clang_createIndex(0, 0);
    
    const char *args[] = {argv[1], "-std=c11"};
    CXTranslationUnit tu = clang_createTranslationUnitFromSourceFile(index, nullptr, 2, args, 0, nullptr);

    if (!tu) {
        std::cerr << "Error: Unable to parse the C file." << std::endl;
        return 1;
    }

    CXCursor root_cursor = clang_getTranslationUnitCursor(tu);
    print_ast(root_cursor, 0);

    for (const auto& var_and_context_in_func : variables_and_context_in_functions) {
        std::cout << "Variable: " << var_and_context_in_func.first << std::endl;
        std::cout << "Function: " << var_and_context_in_func.second.first << std::endl;
        std::cout << "Context:" << std::endl << var_and_context_in_func.second.second << std::endl;
    }

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}
