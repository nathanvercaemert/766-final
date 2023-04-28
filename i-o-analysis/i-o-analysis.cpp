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
    return CXChildVisit_Break;
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

std::vector<std::pair<std::string, std::string>> variables_and_context;

void print_ast(CXCursor cursor, int depth) {
    if (clang_getCursorKind(cursor) == CXCursor_FirstInvalid)
        return;

    for (int i = 0; i < depth; ++i)
        std::cout << "  ";

    CXString cursor_kind_name = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
    CXString cursor_spelling = clang_getCursorSpelling(cursor);
    std::cout << clang_getCString(cursor_kind_name) << " " << clang_getCString(cursor_spelling) << std::endl;
    
    clang_disposeString(cursor_kind_name);
    clang_disposeString(cursor_spelling);

    if (clang_getCursorKind(cursor) == CXCursor_IntegerLiteral) {
        auto res = clang_Cursor_Evaluate(cursor);
        auto int_lit_value = clang_EvalResult_getAsInt(res);
        clang_EvalResult_dispose(res);
        std::cout << int_lit_value << std::endl;
    }

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

    // Read the contents of the file into a string.
    std::ifstream file_stream(argv[1]);
    std::stringstream buffer;
    buffer << "#include <stdio.h>\n#include <sys/types.h>\n" << file_stream.rdbuf();
    std::string file_contents = buffer.str();

    CXIndex index = clang_createIndex(0, 0);

    // Create a CXUnsavedFile structure for the modified file.
    CXUnsavedFile unsaved_file = {
        argv[1],  // filename
        file_contents.c_str(),  // contents
        static_cast<unsigned long>(file_contents.length())  // length
    };

    const char *args[] = {"-std=c11", "-I/usr/include", "-I/usr/local/include", "-fparse-all-comments"};
    CXTranslationUnit tu = clang_parseTranslationUnit(
    index, argv[1], args, 3,
    &unsaved_file, 1,
    CXTranslationUnit_DetailedPreprocessingRecord |
    CXTranslationUnit_IncludeAttributedTypes |
    CXTranslationUnit_VisitImplicitAttributes |
    CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles |
    CXTranslationUnit_KeepGoing |
    CXTranslationUnit_IncludeBriefCommentsInCodeCompletion |
    CXTranslationUnit_RetainExcludedConditionalBlocks |
    CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles
                                                      );

    if (!tu) {
        std::cerr << "Error: Unable to parse the C file." << std::endl;
        return 1;
    }

    CXCursor root_cursor = clang_getTranslationUnitCursor(tu);

    print_ast(root_cursor, 0);

    for (const auto& var_and_context : variables_and_context) {
        std::cout << "Variable: " << var_and_context.first << std::endl;
        std::cout << "Context:" << std::endl << var_and_context.second << std::endl;
    }

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}
