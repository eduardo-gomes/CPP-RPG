#include "inimigo.hpp"
std::string inimigo::get_name() {
	return "IA_name";
}
void inimigo::atack_round(){
	cout << "nobody selected to atack from " << this->get_name() << endl;
}
void inimigo::atack_round(personagem *p){
	std::cout << "IA goes here" << std::endl;
	atack(armas[0], p);
}
void inimigo::create_loot(){}