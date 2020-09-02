#include <iostream>

#include <mwlogger.h>

#include <binfile.h>

#define PROGRAMM_NAME "testbinfile"

int main(int argc, const char* argv[]) {
    init_logger(PROGRAMM_NAME);

    if (argc < 3) {
        cout << "usage: " << argv[0] << " input_file output_file" << endl;
        return EXIT_FAILURE;
    }

    vector<char> char_vector;

    BinFile binfile(argv[1]);

    binfile.open_for_read();
    char_vector = binfile.read();
    binfile.close();
    LOG_INFO(_logger, "read");

    binfile.open_for_write(argv[2]);
    binfile.write(char_vector);
    binfile.close();
    LOG_INFO(_logger, "wrote");

    return EXIT_SUCCESS;
}
