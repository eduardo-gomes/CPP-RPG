#pragma once
#include "personagem.hpp"
namespace CPP_RPG::Server {
namespace menu {
	void rename(std::shared_ptr<jogador> &);
}
class jogador : public personagem {
	private:
	std::string name;
	unsigned int money;
	int sk_life, sk_damage, sk_critcal, sk_actionpoints, sk_magicalpoints;
	unsigned int damage_given, damage_taken, enemies_killeds;
	unsigned int damage_given_all, damage_taken_all, enemies_killeds_all, deaths;
	bool connected;

	public:
	//void set_server_client_socket(server_client_socket *newout){out = newout;};
	void disconect();
	void connect(std::shared_ptr<server_client_socket> &newsocket);
	bool is_connected() { return connected; }
	virtual bool is_jogador() { return 1; };
	std::string get_name();
	void create_loot();
	//int atack_round();
	std::pair<int, int> atack_round(std::shared_ptr<personagem> &);
	void statadd_recieved_damage(int);
	void statadd_given_damage(int);
	void recieve_loot(std::shared_ptr<personagem> &);
	void on_death();
	void heal();
	~jogador();
	jogador();
	jogador(std::string &);
	unsigned int get_money() const { return money; }
	int get_sk_life() const { return sk_life; }
	int get_sk_damage() const { return sk_damage; }
	int get_sk_critcal() const { return sk_critcal; }
	int get_sk_actionpoints() const { return sk_actionpoints; }
	int get_sk_magicalpoints() const { return sk_magicalpoints; }
	friend void from_json(const nlohmann::json &j, jogador *p);
	friend void to_json(nlohmann::json &j, const jogador *p);
	friend void menu::rename(std::shared_ptr<jogador> &);
};
}  // namespace CPP_RPG::Server