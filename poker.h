#define JACK 	11
#define QUEEN 	12
#define KING 	13
#define ACE		14

#define HEARTS		0
#define SPADES		1
#define DIAMONDS	2
#define CLUBS		3

class card{
	int value;
	int suit;
	
public:
	card(int v=0, int s=0){value = v; suit = s;}
	int getValue() const {return value;}
	int getSuit() const {return suit;}
	void setValue(const int new_v) {value = new_v;}
	void setSuit(const int new_s) {suit = new_s;}
};

class hand{
protected:
	int numcards;
	card *cards;
	
public:
	hand();
	hand(const card cpy[], const int size);
	hand(const hand &h);
	~hand();
	void resize(int size);	
	void push_back(const card &c);
	void insert(const int position, const card &c);
	void remove(const int position);
	const int getSize() const {return numcards;}
	void clear();
	card *pop(const int position);
	card *get(const int position);
	const card *get(const int position) const;
	void show_cards();
};

class deck: public hand{
//	hand theDeck;
public:
	deck();
	void shuffle();
	card *draw();
//	draw(pop)
//	check_enough
};	

class player{
	hand playerHand;
	int balance;
	int id;
	int ante;
	bool dealer;
	bool sblind;
	bool bblind;
public:
	player(int money);
	~player(){std::cout << "deleting player";}
	void setBalance(int newBalance){balance = newBalance;}
	bool isdealer(){return dealer;}
	bool isSblind(){return sblind;}
	bool isBblind(){return bblind;}
	void setdealer(bool d){dealer = d;}
	void setSblind(bool s){sblind = s;}
	void setBblind(bool b){bblind = b;}
	void setID(int newID){id = newID;}
	int getBalance(){return balance;}
	int getHandsize(){ return playerHand.getSize();}
	int getID(){return id;}
	void add2hand(const card &c);
	void withdraw(int amount);
	void deposit(int amount){ balance += amount;}
	int action(char mode, int match, int minRaise);
	int raise(int minraise);
	void fold();
	void call(int amount);
	void show_stats();
};

class game{
	player *players;
	player *player0; //keeps track of first player
	player *dealer; //keeps track of dealer
	player *sblind_ptr;
	player *bblind_ptr;
	int numPlayers;
	hand board;
	deck gameDeck;
	int pot;
	int turns; 
	int bbValue; //value of big blind
	int sbValue; //value of small blind
public:
	game();
	~game();
	void setbbValue(int newbb){bbValue= newbb;}
	void setsbValue(int newsb){sbValue= newsb;}
	void setTurns(int newt){turns= newt;}
	void initplayers();
	void blinds();
	void rotateblinds();
	void turn();
	void collectFromBlinds();
	void pre_flop_bet();
	void pre_flop();
	void dealPlayers();
	void removePlayer(int id);
	char getAction(int numofRaises);
	char checkRaiseCount(char mode, int raises);
	int getPot(){return pot;}
	int setPot(int newPot){pot = newPot;}

};


 










