#define TM_CLI_IMPLEMENTATION
// #define TMCLI_USE_WCHAR_T
#include "main.cpp"

enum flag_values {
    MY_FLAG0 = (1u << 0),
    MY_FLAG1 = (1u << 1),
    MY_FLAG2 = (1u << 2),
};

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

int main(int argc, char const* argv[]) {
    unsigned verbose = 0;
    unsigned warning_level = 0;
    unsigned warning_level_counter = 0;
    unsigned flags = 0;
    /* clang-format off */
    static const tmcli_option options[] = {
        {TMCLI_TEXT("t"), TMCLI_TEXT("template-file"), CLI_REQUIRED_ARGUMENT, CLI_REQUIRED_OPTION},
        {TMCLI_TEXT("o"), TMCLI_TEXT("output"), CLI_REQUIRED_ARGUMENT},

        {TMCLI_TEXT("v"), TMCLI_TEXT("verbose"), CLI_NO_ARGUMENT, CLI_OPTIONAL_OPTION, {0},
                                                 CLI_NO_VALIDATOR, CLI_ASSIGN_OUTPUT, &verbose, 1},
        {TMCLI_TEXT("w"), TMCLI_TEXT("warning-level"), CLI_OPTIONAL_ARGUMENT, CLI_OPTIONAL_OPTION,
         /*.multiple=*/{4, &warning_level_counter}, CLI_ARGUMENT_UINT, CLI_COUNTER_OUTPUT, &warning_level, 2},

        {TMCLI_TEXT("a"), TMCLI_TEXT("flag0"), CLI_NO_ARGUMENT, CLI_OPTIONAL_OPTION, {0},
                                               CLI_NO_VALIDATOR, CLI_FLAG_OUTPUT, &flags, MY_FLAG0},
        {TMCLI_TEXT("b"), TMCLI_TEXT("flag1"), CLI_NO_ARGUMENT, CLI_OPTIONAL_OPTION, {0},
                                               CLI_NO_VALIDATOR, CLI_FLAG_OUTPUT, &flags, MY_FLAG1},
        {TMCLI_TEXT("c"), TMCLI_TEXT("flag2"), CLI_NO_ARGUMENT, CLI_OPTIONAL_OPTION, {0},
                                               CLI_NO_VALIDATOR, CLI_FLAG_OUTPUT, &flags, MY_FLAG2},
    };
    /* clang-format on */

    auto parser = tmcli_make_parser_ex(argv[0], argc - 1, argv + 1, options, sizeof(options) / sizeof(options[0]),
                                       {stderr, true});

    tmcli_parsed_option parsed_option = {};
    while (tmcli_next(&parser, &parsed_option)) {
        if (parsed_option.option) {
            if (parsed_option.argument) {
                printf("Option --%s given: %s.\n", parsed_option.option->long_option, parsed_option.argument);
            } else {
                printf("Option --%s given.\n", parsed_option.option->long_option);
            }
        } else {
            printf("Free argument given: %s.\n", parsed_option.argument);
        }
    }
    if (!tmcli_validate(&parser)) {
        return -1;
    }
    printf("Option --warning-level was given %u times and amounts to %u.\n", warning_level_counter, warning_level);
    printf("Verbose flag is set to %u.\n", verbose);
    printf("Successful commandline parsing.\n");

    return 0;
}