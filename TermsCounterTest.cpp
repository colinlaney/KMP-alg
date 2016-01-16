#include "TermsCounter.h"

#include <cstdarg>
#include <iostream>
#include <sstream>

#include <assert.h>

const char* OK_STR = "\e[32mOK\e[0m";
const char* FAIL_STR = "\e[32mFAIL\e[0m";

bool _testTermsCounter(const std::string& text,
                     const std::vector<std::string>& terms,
                     const std::vector<unsigned>& answer) {
	std::cout << answer.size() << "    " << terms.size() << std::endl;
    assert(answer.size() == terms.size());

    TermsCounter termsCounter(terms);

    std::vector<unsigned> result =
        termsCounter.computeOccurrencesCount(text);

    std::vector<unsigned> result2 =
        termsCounter.computeOccurrencesCount(text);

    if (result.size() != answer.size()) {
        std::cerr << "    The size of the result vector is incorrect: "
                  << result.size() << " instead of " << answer.size()
                  << std::endl;
        return false;
    }

    if (result.size() != result2.size()) {
        std::cerr << "    The TermsCounter returns different results"
                  << " for the same text" << std::endl;
        return false;
    }

    for (size_t i = 0; i < answer.size(); ++i) {
        if (result[i] != answer[i]) {
            std::cerr << "    Occurrences count for \"" << terms[i]
                      << "\" is incorrect: " << result[i]
                      << " instead of " << answer[i]
                      << std::endl;
            return false;
        }
        if (result[i] != result2[i]) {
            std::cerr << "    The TermsCounter returns different results"
                      << " for the same text" << std::endl;
            return false;
        }
    }

    return true;
}

bool testTermsCounter(const char* name, const char* text, ...) {
    std::vector<std::string> terms;
    std::vector<unsigned> answer;

    va_list ap;
    va_start(ap, text);

    while(true) {
        const char* term = va_arg(ap, const char*);
        if (term == NULL)
            break;
        terms.push_back(term);
        answer.push_back(va_arg(ap, unsigned));
    }

    va_end(ap);

    std::cout << name << ":" << std::endl;
    bool ok = _testTermsCounter(text, terms, answer);
    std::cout << "    " << (ok ? OK_STR : FAIL_STR) << std::endl;

    return ok;
}

int main(void) {

    bool ok = true;

    /* NOTE: This set of tests is NOT complete.
     *       We suggest you to extend it. */

    ok &= testTermsCounter("Test1", 
            "This is an example term",
            "is", 1,
            "example term", 1,
            NULL);

    ok &= testTermsCounter("Test2",
            "This is not a \n\n term.",
            "a term", 0,
            NULL);

    ok &= testTermsCounter("Test3",
            "This this this this",
            "this this", 2,
            NULL);

    ok &= testTermsCounter("Test4",
            "This is not a Term.",
            "a term", 1,
            NULL);

    ok &= testTermsCounter("Test5",
            "This is not a   term.",
            "a term", 1,
            NULL);

    ok &= testTermsCounter("Test6",
            "This is not a          term.",
            "a term", 1,
            NULL);

    std::cout << "\nOverall:\n    " << (ok ? OK_STR : FAIL_STR)
              << std::endl;

    return ok ? 0 : 1;
}

