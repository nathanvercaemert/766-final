#include <iostream>
#include <fstream>
#include <functional>
#include <clang-c/Index.h>

void traverse_ast(CXCursor cursor, int depth, const std::function<void(CXCursor, int)>& process) {
    process(cursor, depth);
    clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
        auto context = reinterpret_cast<std::pair<int, const std::function<void(CXCursor, int)>*>*>(client_data);
        traverse_ast(c, context->first + 1, *context->second);
        return CXChildVisit_Continue;
    }, &std::make_pair(depth, &process));
}

std::string get_context(CXSourceLocation location, int before, int after) {
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
    traverse_ast(root_cursor, 0, [](CXCursor cursor, int depth) {
        for (int i = 0; i < depth; ++i)
            std::cout << "  ";
        CXString cursor_kind_name = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
        CXString cursor_spelling = clang_getCursorSpelling(cursor);
        std::cout << clang_getCString(cursor_kind_name) << " " << clang_getCString(cursor_spelling) << std::endl;
        clang_disposeString(cursor_kind_name);
        clang_disposeString(cursor_spelling);
    });

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}
