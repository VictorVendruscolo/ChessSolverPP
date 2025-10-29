#pragma once

enum Cor { BRANCA, PRETA};

class tabuleiro;

class peca {
protected:
	Cor cor;
	int x, y;
public: 
	peca(Cor cor, int x, int y) {};
	virtual ~peca() {}
};