#include <iostream>
#include <fstream>
#include <map>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <cctype>

using namespace std;

static unordered_map<string, int> m[26];

static int cal_(const string &s) {
  int score = 0;
  for(auto ch : s) switch(ch) {
    default: score += 1; break;
	case 'd':
	case 'g': score += 2; break;
	case 'b':
	case 'c':
	case 'm':
	case 'p': score += 3; break;
	case 'f':
	case 'h':
	case 'v':
	case 'w':
	case 'y': score += 4; break;
	case 'k': score += 5; break;
	case 'j':
	case 'x': score += 8; break;
	case 'q':
	case 'z': score += 10; break;
  }
  return score;
}

static string fetch_word(char lc) {
  int minlen[] = {5, 4, 0};
  string ss;

  unordered_map<string, int> &rm = m[lc - 'a'];

  for(int iter = 0; iter < 3; ++iter) {
    int nw = 0;
    pair<int, string> vs[5];

    for(const auto &it : rm) {
	  if((int)it.first.size() < minlen[iter]) continue;
      vs[nw] = make_pair(it.second, it.first);

	  if(nw < 4) ++nw;
	  else sort(&vs[0], &vs[5]);
    }
    if(0 == nw) continue;
    int id = rand() % nw;
    ss = vs[id].second;
    rm.erase(ss);
	break;
  }
  return ss;
}

int main() {
  std::ifstream ifs1("freq.txt");
  std::ifstream ifs2("allwords.txt");
  if(!ifs1 && !ifs2) {
    cerr << "dictionary not found\n";
	return -1;
  }

  long long int score = 0LL;
  long long int comscore = 0LL;
  char choice;
  string s;
  int rank = 50000;

  while(ifs2 >> s) {
    bool good = true;
	for(const char ch : s) {
	  if('a' > ch || ch > 'z') {
	    good = false;
		break;
	  }
	}
	if(good && (s.size() > 0)) m[s[0] - 'a'][s] = rank;
  }

  rank = 0;
  while(ifs1 >> s) {
    bool good = true;
	for(const char ch : s) {
	  if('a' > ch || ch > 'z') {
	    good = false;
		break;
	  }
	}
    if(good && (s.size() > 0)) m[s[0] - 'a'][s] = ++rank;
  }

  srand( time(0) );

  char lc = (rand() % 26) + 'a';
  string w = fetch_word(lc);
  comscore += cal_(w);

  cout << "Welcome. Your name is [YOU] and my name is [ME]\n";
  cout << "Make your next word starting with the last letter of my word\n";
  cout << "Score you get is as following\n\n";

  cout << "(1 point)-A, E, I, O, U, L, N, S, T, R\n";
  cout << "(2 points)-D, G\n";
  cout << "(3 points)-B, C, M, P\n";
  cout << "(4 points)-F, H, V, W, Y\n";
  cout << "(5 points)-K\n";
  cout << "(8 points)- J, X\n";
  cout << "(10 points)-Q, Z\n\n";

  cout << "[ME] " << w << '\n';
  lc = w[w.size()-1];

  for(; /* ever */ ;) {
    string ss;
	do {
	  if(ss.size() > 0) cout << "Error, try again\n";
	TBA:
	  cout << "[YOU] ";
	  cout.flush();
      cin >> ss;
	  for(char &ch : ss) ch = tolower(ch);
	  if(ss[0] != lc) {
	    cout << "Please start with " << lc << '\n';
	    goto TBA;
	  }
	} while(m[int(lc - 'a')].find(ss) == m[int(lc - 'a')].end());

    score += cal_(ss);
    m[int(lc - 'a')].erase(ss);
    score += (int)ss.length();
	lc = ss[ss.size() - 1];
	cout << "Score is: you " << score << ", me " << comscore << "\n\n";
    
    cout<<"Wanna Quit press(y) for quit\n";
    cin>>choice;
	
    if(choice=='y'||choice=='Y'){
	if(score>comscore){
		cout<<"*** Congratulations. You won As you have more points ***\n\n";
		break; 
    	}
    	else if(score<comscore){
    		cout<<"You lost!, You have lesser points than computer \n\n";
    		break;
	}
	else{
		cout<<"Scores are level, Match Draw\n\n";
		break;
	}
     }

    w = fetch_word(lc);
    string w = fetch_word(lc);
	lc = w[w.size() - 1];
	if(w.size() == 0) {
	  cout << "*** Congratulations. You won. ***";
	  break;
	}
	comscore += cal_(w);
	cout << "[ME] " << w << '\n';
	if(m[lc - 'a'].size() == 0) {
	  cout << "ALAS:: You lost, no more word that begins with " << lc << '\n';
	  break;
	}
  }
}

