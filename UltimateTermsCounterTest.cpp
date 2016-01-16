#include "TermsCounter.h"

#include <iostream>
#include <assert.h>
#include <stdarg.h>

const char* OK_STR = "\e[32mOK\e[0m";
const char* FAIL_STR = "\e[31mFAIL\e[0m";

bool _testTermsCounter(const std::string& text,
                     const std::vector<std::string>& terms,
                     const std::vector<unsigned>& answer) {

    assert(answer.size() == terms.size());

    TermsCounter termsCounter(terms);
    std::vector<std::string> tt = termsCounter.getTerms();
    //tt[0] = "test";
    for(int i=0; i < tt.size(); i++)
    std::cout << tt[i] << std::endl;

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

    unsigned total = 0;
    unsigned succeed = 0;

    /* NOTE: This set of tests is NOT complete.
     *       We suggest you to extend it. */

    total += 1;
    succeed += testTermsCounter("SingleWordSingleTerm", 
            "This is an example term x",
            "This", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("SingleWord", 
            "This is an example term x",
            "is", 1,
            "example", 1,
            "term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("MultiWordSingleTerm",
            "This is a term x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("MultiWord",
            "This is a term x",
            "a term", 1,
            "This is", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("SkipSpaces",
            "this is a    term x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("SkipSpaceLike",
            "this is a  \t  term x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("SkipNewline",
            "this is a  \n  term x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("SkipTwoNewline",
            "this is a  \n \n  term x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("DontSkipParBreak",
            "this is a  \n\n  term x",
            "a term", 0,
            NULL);

    total += 1;
    succeed += testTermsCounter("DontSkipTwoParBreaks",
            "this is a  \n\n  term. this is a \n\n term x",
            "a term", 0,
            NULL);

    total += 1;
    succeed += testTermsCounter("SkipNewlinesAfterTwoParBreaks",
            "this is a  \n\n  term this is a \n\n term this is a \n term x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("CaseInsensitivity",
            "tHiS Is A tErM x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("BreakOnSpecial",
            "this is a, term x",
            "a term", 0,
            NULL);

    total += 1;
    succeed += testTermsCounter("BreakOnMultipleSpecial",
            "this is a, term, a, term, a term, a, term, x",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("WordBeforeSpecial",
            "this is a,term x",
            "a", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("WordAfterSpecial",
            "this is a,term x",
            "term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("OverlapJoin",
            "x this is a term x",
            "this is", 1,
            "is a", 1,
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("OverlapInclude",
            "x this is a term x",
            "this is", 1,
            "is a", 1,
            "this is a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("OverlapSelf",
            "x this this this this x",
            "this this", 2,
            NULL);

    total += 1;
    succeed += testTermsCounter("DontIncludeSubwords",
            "x This is a subword x",
            "word", 0,
            NULL);

    total +=1;
    succeed += testTermsCounter("SpacesAtStart",
            "  this is a term",
            "this is", 1,
            NULL);

    total +=1;
    succeed += testTermsCounter("AtStart",
            "this is a term",
            "this is", 1,
            NULL);

    total +=1;
    succeed += testTermsCounter("AtEnd",
            "this is a term",
            "a term", 1,
            NULL);

    total += 1;
    succeed += testTermsCounter("LongTest",
            "this is an example sentence that contains many words. "
            "Some words repeat several times. It also contains subwords."
            "And\n\nparagraph breaks.",
            "this is", 1,
            "is", 1,
            "is an example", 1,
            "words", 2,
            "contains", 2,
            NULL);

    std::cout << "\nOverall:\n    " << (succeed == total ? OK_STR : FAIL_STR)
              << std::endl;
    
    std::cout << (total - succeed) << " tests failed out of " << total
              << std::endl;

    return total - succeed;
}

