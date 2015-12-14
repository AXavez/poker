#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "poker.h"

#define STARTING_BALANCE	200

using namespace std;

int random_gen(int i) {return rand()%i;} //random generator used to shuffle cards

hand::hand(){
	numcards = 0;
	cards = new card[0];
}

hand::hand(const card cpy[], const int size){
	cards = new card[size-1];
	
	for(numcards=0; numcards < size; numcards ++){
		cards[numcards] = cpy[numcards];	
	}
	numcards ++;
}

hand::hand(const hand &h){
	cards = new card[h.numcards-1];
	
	for(numcards=0; numcards < h.numcards; numcards++){
		cards[numcards] = h.cards[numcards];	
	}
	numcards++;
}

hand::~hand(){
	delete [] cards;
}

void hand::resize(int size){
	card *newHand = new card[size];
	
	int min_len = (numcards<size ? numcards : size);

	for(int i=0; i<min_len; i++){
		newHand[i] = cards[i];
	}
	numcards = size;
	delete[] cards;
	cards = newHand;
}

void hand::push_back(const card &c){
	resize(numcards+1);
	cards[numcards-1]=c;
}

void hand::insert(const int position, const card &c){
	resize(numcards+1);
	for(int i=0; numcards-2-i > position; i++){
		cards[i] = cards[i+1];
	}
	cards[position] = c;
}

void hand::remove(const int position){
	for(int i=position; i<numcards-1;i++){
	//write over desired position
		cards[i] = cards[i+1];	
	}
	resize(numcards-1); //will remove last position, unneeded
}

void hand::clear(){
	resize(0);
}

card *hand::pop(const int position){
	card *poppedcard = new card;
	*poppedcard = *(cards+position);
	remove(position);
	return poppedcard;
}

card *hand::get(const int position){
	if(position>numcards) return NULL;
	return cards+position;
}

const card *hand::get(const int position) const{
	if(position>numcards) return NULL;
	return cards+position;
}

void hand::show_cards(){
	int val, sui;
	cout << "Cards:\n";
	for(int i =0; i < numcards; i++){
		val = cards[i].getValue();
		sui = cards[i].getSuit();
		
		switch(val){
			case 11:
				cout << "Jack of ";
				break;
			case 12:
				cout << "Queen of ";
				break;
			case 13:
				cout << "King of ";
				break;
			case 14:
				cout << "Ace of ";
				break;
			default:
				cout << val << " of ";
				break;		
		}
		switch(sui){
			case 0:
				cout << "Hearts\n";
				break;
			case 1:
				cout << "Spades\n";
				break;
			case 2:
				cout << "Diamonds\n";
				break;
			case 3:
				cout << "Clubs\n";
				break;
			default:
				cout << "error with suits\n";
				break;
		}
	
	}


}

deck::deck():hand(){
	resize(52);
	int cardEntry = 0;
	//loops to enter card values
	for(int i=0; i <4; i++){//suits
		for(int j=2; j<=14; j++){//values
			get(cardEntry)->setValue(j);
			get(cardEntry)->setSuit(i);	
			cout << cardEntry << " created, value " << j << " suit " << i << '\n';		
			cardEntry++;
		}
	}
	cout << "created deck\n";
}

//deck::deck(){
//	theDeck.resize(52);
//	int cardEntry = 0;
//	
//	//loops to enter card values
//	for(int i=0; i <4; i++){//suits
//		for(int j=2; j<=14; j++){//values
//			theDeck.get(cardEntry)->setValue(j);
//			theDeck.get(cardEntry)->setSuit(i);	
//			cout << cardEntry << " created, value " << j << " suit " << i << '\n';		
//			cardEntry++;
//		}
//	}
//	cout << "created deck\n";
/////////////////////////////////////////////////////	
	//checking deck creation
/////////////////////////////////////////////////////
//	int v, s;
//	cardEntry = 0;
//	for(int i=0; i <4; i++){//suits
//		for(int j=2; j<=14; j++){//values
//			v = theDeck.get(cardEntry)->getValue();
//			s = theDeck.get(cardEntry)->getSuit();
//			cout << "card " << cardEntry << " value " << v << " suit " << s << '\n';
//			cardEntry++;
//		}
//	}
//}

void deck::shuffle(){
	srand(unsigned(time(0))); //generate random number
	random_shuffle(get(0), get(getSize()), ::random_gen);
	cout << "deck shuffled\n";
	
	int v, s;
	int cardEntry = 0;
	for(int i=0; i <4; i++){//suits
		for(int j=2; j<=14; j++){//values
			v = get(cardEntry)->getValue();
			s = get(cardEntry)->getSuit();
			cout << "card " << cardEntry << " value " << v << " suit " << s << '\n';
			cardEntry++;
		}
	}
}

card *deck::draw(){
	return pop(0);
//return card
}

player::player(int money=0){
	playerHand.resize(0);
	balance = money;
	setdealer(false);
	setSblind(false);
	setBblind(false);
}

void player::add2hand(const card &c){
	playerHand.push_back(c);
}

void player::withdraw(int amount){
	if(balance - amount < 0) {
		ante = balance;
		balance = 0;
	}
	else{
		balance -= amount;
		ante = amount;
		pot += amount;
	}
	
}

int player::raise(int minraise){
	int amount = 0;
	while(amount < minraise){
		cout << "Enter an amount to raise to, minimum is  " << minraise << ": ";
		cin >> amount;
		cout << '\n';
	}
	cout << "Player " << id << " raises by " <<  amount << '\n';
	withdraw(amount);
	return amount;
}

void player::fold(){
	playerHand.resize(0);
	cout << "Player " << id << " folds.\n";
}

void player::call(int amount){
	withdraw(amount);
	cout << "Player " << id << " calls\n";
}

int player::action(char mode, int minCall, int minRaise){ //return new min bet
	if(mode == 'f') {
		fold();
		return minCall;
	}
	if(mode ==  'c') {
		call(minCall);
		return minRaise;
	}
	if(mode == 'r') {return raise(minRaise);}

}

void player::show_stats(){
	cout << "\n\nPlayer " << getID() << ":";
	if(isdealer()){ cout << "dealer";}
	if(isSblind()){ cout << "small blind";}
	if(isBblind()){ cout << "big blind";}
	
	cout <<"\nbalance = " << getBalance() << '\n';
	playerHand.show_cards();
}

game::game(){
	initplayers();
	players = new player[numPlayers];
	for(int i=0; i < numPlayers; i++){//do this for all players
		players[i].setBalance(STARTING_BALANCE);
		players[i].setID(i);
	}
	player0=&players[0]; //keeps track of the first player
	dealer = &players[0]; //keeps track of dealer
	if(numPlayers == 2){
		sblind_ptr = &players[0];
		bblind_ptr = &players[1];
	}
	else{
		sblind_ptr = &players[1];
		bblind_ptr = &players[2];
	}
	
	blinds();
	gameDeck.shuffle();
	setbbValue(2);
	setsbValue(1);
	setTurns(1);
	
}

game::~game(){
	cout << "\nFinishing Game\n";	
}

void game::initplayers(){
	numPlayers = 0;
	while(numPlayers < 2 || numPlayers >= 7){
		cout << "Enter number of players(2-6): ";
		cin >> numPlayers;
		cout << '\n';
	}
}

void game::blinds(){
	for(int i=0; i < numPlayers; i++){
		//if(dealer == sblind_ptr == &players[i]){
		if(dealer == &players[i] && sblind_ptr == &players[i]){
			players[i].setdealer(true);
			players[i].setSblind(true);
			players[i].setBblind(false);
		}
		
		else if(dealer = &players[i]){
			players[i].setdealer(true);
			players[i].setSblind(false);
			players[i].setBblind(false);
		}

		else if(sblind_ptr = &players[i]){
			players[i].setdealer(false);
			players[i].setSblind(true);
			players[i].setBblind(false);
		}
		
		else if(bblind_ptr = &players[i]){
			players[i].setdealer(true);
			players[i].setSblind(false);
			players[i].setBblind(true);
		}
		
		else{
			players[i].setdealer(false);
			players[i].setSblind(false);
			players[i].setBblind(false);
		}
	}
}

void game::rotateblinds(){
	//find dealer
	int dealerID = dealer->getID();
	dealer = &players[(dealerID+1)%numPlayers];
	int sbID = sblind_ptr->getID();
	sblind_ptr = &players[(sbID+1)%numPlayers];
	int bbID = bblind_ptr->getID();
	bblind_ptr = &players[(bbID+1)%numPlayers];
	
	blinds();
}

void game::dealPlayers(){
	cout << numPlayers << '\n';
	
	for(int i=0; i < numPlayers; i++){
		players[i].add2hand(*gameDeck.draw());
		players[i].add2hand(*gameDeck.draw());
		players[i].show_stats();
	}
}

void game::collectFromBlinds(){
	sblind_ptr->withdraw(sbValue);
	bblind_ptr->withdraw(bbValue);
}

char game::getAction(int numofRaises){
	char mode = '\0';
	while( mode != 'c' || mode != 'f' || mode != 'r'){
		cout << "Enter c to call, f to flop, or r to raise";
		cin >> mode;	
		cout << '\n';
		mode = checkRaiseCount(mode, numofRaises);		
	}
	return mode;
}

char game::checkRaiseCount(char mode, int raises){
	if(raises >=3 && mode == 'r') {
		cout << "There has already been 3 raises, please choose a different action.\n";
		return '\0'; //change mode to force another prompt
	}
	return mode; //default change nothing
}

void game::pre_flop_bet(){
	int bbID = bblind_ptr->getID(); //get player id of big blind 
	int curr = (bbID+1)%numPlayers; //first bet should be left of the big blind
	char mode;
	int min_bet = bbvalue; //minimum bet value is big blind value 
	int new_bet;
	int numofRaises= 0; // can only be three number of raises max
	numCalled = 0; //set number of player who called to 0; used to signal next turn
	while(numCalled != numPlayers){
		if(players[curr].getHandsize() == 0) {
			numCalled++; // if player folds, add to counter to not effect 
			continue; //skip over player if folded
		}
		if( sblind_ptr == & players[curr]){
			mode = getAction(numofRaises);
			new_bet = players[curr].action(mode, sbvalue, bbvalue); //
		}
		else if( bblind_ptr == &players[curr]){
			mode = getAction(numofRaises);
			if(min_bet == bbvalue){ //no raises after blind collection
				
			new_bet = players[curr].action(mode, 
		}
		else {
			mode = getAction(numofRaises);
			new_bet = players[curr].action(mode, bbvalue, bbvalue);
		}
		if( mode == 'c'){ numCalled++;}
		else if(mode == 'r'){ 
			numofRaises++;
			numCalled =1;
		} //reset numCalled counter because of raise 
		curr = (curr+1)%numPlayers;
	}
}

void game::pre_flop(){
//rotate blinds after first turn
	if(turns > 1){ rotateblinds();} 
	pot = 0;
	collectFromBlinds();
	dealPlayers();
	pre_flop_bet();
	
	
	
}

void game::turn(){
	pre_flop();
	
//put money in pot for blinds
	
	//betting pre flop
	//flop
	//betting
	//turn
	//betting
	//river
	cout << "Starting turn \n";
}

void game::removePlayer(int id){
	int arrayID;
	for(arrayID=0;arrayID < numPlayers;arrayID++){
	//searches for player in array that matches ID
		if(players[arrayID].getID() == id){
			players[arrayID].~player();
			break; //searches for player in array
		}
	}
	

	numPlayers--; //decrease number of players
	player *newplayers = new player[numPlayers];
	
	for(int i=0; i<numPlayers; i++){
		if(i>=arrayID){
			newplayers[i] = players[i+1];
			if(player0== &players[i+1]){player0 = &newplayers[i];} //set new player0
		}
		else {
			newplayers[i] = players[i];
			if(player0 == &players[i]){player0 = &newplayers[i];}
		}
	}
	
	delete[] players;
	players = newplayers;
}

int main(){
	game g1;
	g1.dealPlayers();
	cout << "done\n";

	return 0;
}
