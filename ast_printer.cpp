#include <iostream>
#include <clang-c/Index.h>

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
    CXTranslationUnit tu = clang_createTranslationUnitFromSourceFile(index, argv[1], 0, nullptr, 0, nullptr);

    if (!tu) {
        std::cerr << "Error: Unable to parse the C file." << std::endl;
        return 1;
    }

    CXCursor root_cursor = clang_getTranslationUnitCursor(tu);
    print_ast(root_cursor, 0);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}
