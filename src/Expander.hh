#ifndef EXPANDER_HH
#define EXPANDER_HH

#include <vector>

#include "Lexicon.hh"
#include "Hmm.hh"
#include "Acoustics.hh"

class Expander {
public:
  // FIXME! Would it be much faster to use shorts and floats here?
  class Word {
  public:
    Word() : 
      avg_log_prob(0), log_prob(0), frames(-1), word_id(-1), active(false) { }
    double avg_log_prob;
    double log_prob;
    int frames;
    int word_id;
    bool active;
  };

  struct WordCompare {
    bool operator()(const Word *a, const Word *b) {
      return a->avg_log_prob > b->avg_log_prob;
    }
  };

  Expander(const std::vector<Hmm> &hmms, Lexicon &lexicon,
	   Acoustics &m_acoustics);
  void sort_best_tokens(int tokens);
  void keep_best_tokens(int tokens);
  Lexicon::Token *token_to_state(const Lexicon::Token *source_token,
				 Lexicon::State &source_state,
				 Lexicon::State &target_state,
				 double new_log_prob);
  void move_all_tokens();
  void clear_tokens();
  void create_initial_tokens(int start_frame);
  void expand(int start_frame, int frames);
  void set_token_limit(int limit) { m_token_limit = limit; }
  void set_max_state_duration(int duration) 
    { m_max_state_duration = duration; }
  void debug_print_history(Lexicon::Token *token);
  void debug_print_timit(Lexicon::Token *token);
  void debug_print_tokens();
  std::vector<Lexicon::Token*> &tokens() { return m_tokens; }
  std::vector<Word*> &words() { return m_sorted_words; }

private:
  const std::vector<Hmm> &m_hmms;
  Lexicon &m_lexicon;
  Acoustics &m_acoustics;

  int m_token_limit;
  int m_max_state_duration;
  std::vector<Lexicon::Token*> m_tokens;
  std::vector<Word> m_words;
  std::vector<Word*> m_sorted_words;
  int m_frame; // Current frame relative to the start frame.

};

#endif /* EXPANDER_HH */
