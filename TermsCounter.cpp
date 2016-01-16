#include "TermsCounter.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <map>
#include <list>
//#include <unordered_map>

TermsCounter::TermsCounter(const std::vector<std::string>& terms){
	mTerms = terms;
	//mTerms.assign(terms.begin(), terms.end());	// S. Meyers, tip 5
}
TermsCounter::~TermsCounter(){}

const std::vector<std::string>& TermsCounter::getTerms() const{
	return mTerms;
}

struct tree {
	std::map<char, tree *> key;
	unsigned* count = NULL;	// counter of a leaf in a terms tree
};

inline bool whitespaces(char i, char j) {
	return isspace(i) && isspace(j);
};

inline tree* buildTree(std::vector<std::string> terms, std::vector<unsigned> *termsCounts) {
	tree *root = new tree;
	tree *node = root;
	for(std::vector<std::string>::iterator term = terms.begin(); term != terms.end(); term++) {
		for(std::string::iterator letter = (*term).begin(); letter != (*term).end(); letter++) {
			if(node->key[*letter]) {
				node = node->key[*letter];
			} else {
				node = node->key[*letter] = new tree;
				node->count = NULL;
			}
		}
		node->count = &(*termsCounts)[term - terms.begin()];
		node = root;
	}
	return root;
}

inline void countTerms(std::string clause, tree *root) {
	std::vector<tree*> *nodesOnTree = new std::vector<tree*>;
	std::vector<tree*>::iterator node;
/* scan the clause: get a letter */
	for(std::string::iterator letter = clause.begin(); letter != clause.end(); ++letter) {
		if(nodesOnTree->empty()){
			nodesOnTree->push_back(root);
		}
/* search the letter in a tree of the terms */
		for(node = nodesOnTree->begin(); node != nodesOnTree->end() && !nodesOnTree->empty(); node++) {
			if(*node == root && letter-clause.begin() && !isspace(*(letter-1))) {	// subword elimination
				node = nodesOnTree->erase(node);
				continue;
			}

			if(isspace(*letter)) {
				if((*node)->count) {
					(*(*node)->count)++;
					node = nodesOnTree->erase(node);
				}
				if(root->key[*(letter+1)]) {	// potential new term
					// node = nodesOnTree->insert(node, root);	//only after count
				}
			}

			if(!nodesOnTree->empty())
			if((*node)->key[*letter]) {	// letter is in tree
				(*node) = (*node)->key[*letter];
			}
			else {	// letter isn't in tree
				node = nodesOnTree->erase(node);
			}
		}
	}
	for(node = nodesOnTree->begin(); node != nodesOnTree->end(); node++)
		if((*node)->count) {	// letter is in the end of term
			(*(*node)->count)++;
		}
	delete nodesOnTree;
}



std::vector<unsigned> TermsCounter::computeOccurrencesCount(
             const std::string& text) const{
	std::string textCopy = text;
	std::string::iterator it;
	std::vector<std::string> clauses;
	std::vector<std::string> Terms = mTerms;
	std::vector<unsigned> counts(mTerms.size(), 0);

	tree *root = new tree;
	tree *key = root;

	std::transform(textCopy.begin(), textCopy.end(), textCopy.begin(), ::tolower);
	for(int j=0; j < Terms.size(); j++)
		std::transform(Terms[j].begin(), Terms[j].end(), Terms[j].begin(), ::tolower);

	it = textCopy.begin();
	while((it = std::search_n(it, textCopy.end(), 2, '\n')) != textCopy.end())
		*it = '.';	// "\n\n" -> ".\n"

	it = textCopy.begin();
	std::replace_if(textCopy.begin(), textCopy.end(), isspace, ' ');
	while((it = std::adjacent_find(it, textCopy.end(), whitespaces)) != textCopy.end())
		textCopy.erase(it);	// delete whitespace repeats

	std::size_t found = textCopy.find_first_not_of(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	std::size_t cur = textCopy.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	if(found)
		clauses.push_back(textCopy.substr(cur, found));
	while (found!=std::string::npos)
	{
		cur = textCopy.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", found);
		found=textCopy.find_first_not_of(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", cur);
		if(cur != found)
			clauses.push_back(textCopy.substr(cur, found-cur));
	}

	root = buildTree(Terms, &counts);

	for(std::vector<std::string>::iterator clause = clauses.begin(); clause != clauses.end(); clause++) {
		countTerms(*clause, root);
	}
	
    return counts;
}

