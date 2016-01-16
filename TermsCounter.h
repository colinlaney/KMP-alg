#ifndef __TERMS_COUNTER_H
#define __TERMS_COUNTER_H

#include <string>
#include <vector>

// A TermsCounter class holds a large number of terms and computes a number of
// occurrences of each term in a large ASCII text. A typical use case of the
// class is to create one instance of it for a large (but fixed) list of terms
// and to use this instance to compute the occurrences of these terms in
// multiple texts (for an example, see the bonus exercise at the end of the
// description).
//
// A term is a sequence of words (made of alphanumeric characters, as defined
// by the isalnum() function) separated by spaces.
//
// A text is a sequence of words separated by non-alphanumeric characters.
//
// A term occurs in the text if all its words are found in the text in the same
// sequence separated by any number of space-like characters (as defined by the
// isspace() function) but not paragraph breaks (a newline followed by another
// newline, "\n\n").
//
// The comparison of words is case-insensitive.
//
// If several different terms overlap in a text, an occurrence is counted for
// all of the overlapping terms. However, a term that overlaps with itself
// should not be double-counted.
//
// For example, the term "Computer science" occurs in a text "computer
// science", "computer   science" and "computer\nscience" but neither in
// "computer, science" nor in "computer\n\nscience". In this example, your
// implementation of TermsCounter::computeOccurrencesCount() should return 1
// occurrence for the former three texts and zero occurrences for the latter
// two texts.
//
// You are allowed to use standard C and C++ libraries (including STL with
// TR1).  For the bonus exercise you can use POSIX calls.
//
// Remember the following priorities, in decreasing order (from most important
// to least important):
//
// P1 - Your code compiles cleanly by typing 'make' in this directory (we use
// GNU Make version 3.81 and GCC version 4.2.1, no other dependencies are
// allowed).
//
// P2 - Your code is correct.  We expect you will test your code.  Segfaults,
// infinite loops, wrong results, etc. are considered major problems.
//
// P3 - Your code has to be optimized for the maximum performance in the use
// case described above.
//
// P4 - Your code should use as small amount of memory as possible in the use
// case described above (but the performance has a higher priority).
//
// Bonus exercise: implement a tool that, given a large list of terms (in a
// text file, each term on its own line) and a folder, finds N most frequently
// occurring terms in all files from that folder (and its subfolders,
// recursively).
//
class TermsCounter {
public:
    // Constructor. Builds the TermsCounter from a list of terms.
    TermsCounter(const std::vector<std::string>& terms);

    // Destructor
    ~TermsCounter();

    // Get a list of terms, in the same order as specified when constructing
    // the object.
    const std::vector<std::string>& getTerms() const;

    // Computes a number of occurrences of each term in a text.
    //
    // Returns a vector of computed number of occurrences, in order of terms
    // specified when constructing the object.
    //
    // This function should not change the state of the object, therefore it is
    // declared as const. This ensures that one object may be used to compute
    // occurrences in several texts.
    //
    std::vector<unsigned> computeOccurrencesCount(
            const std::string& text) const;

private:
    // ...
    std::vector<std::string> mTerms;
};

#endif

