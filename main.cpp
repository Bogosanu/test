#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <array>



//const int rowsize = 16; //variabila momentan globala recomandata in feedback


class Line {
private:
    int x;
    int len;
public:
    Line(int x_, int len_) : x{x_}, len{len_} {}
    Line(){
        x=5;
        len=6;
    };




    int getx() const { return x; }
    int getlen() const { return len; }

    friend std::ostream& operator<<(std::ostream& os, const Line& ln) {
        os << "Pozitie:  " << ln.x << "; Lungime: " << ln.len << "\n";
        return os;
    }



};

class Ball {
private:
    int x,y;
public:
    Ball(int x_, int y_) : x{x_}, y{y_} {}

    int getx() const { return x; }
   // int gety() const { return y; }

    friend std::ostream& operator<<(std::ostream& os, const Ball& ba) {
        os << "Coordonate x/y: " << ba.x << " " << ba.y << "\n";
        return os;
    }

};





class Brick {
private:
    int hp;
    int x,y;
    bool alive;
public:
    Brick()
    {
        hp = 0;
        x = 0;
        y = 0;
        alive = 0;

    }
    Brick(int hp_, int x_, int y_, bool alive_) : hp{hp_}, x{x_}, y{y_}, alive{alive_} {}
    Brick(const Brick& other) : hp{other.hp}, x{other.x}, y{other.y}, alive{other.alive} {}  //constructor de copiere

    int gethp() const { return hp; }
    int getx() const { return x; }
 //   int gety() const { return y; }
 //   int isalive() const { return alive; }




    Brick& operator=(const Brick& other) {
        hp = other.hp;
        x = other.x;
        y = other.y;
        alive = other.alive;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Brick& br) {
        os << "Health points: " << br.hp << "; Coordonate x/y: " << br.x << " " << br.y << "; Status: " << br.alive << "\n";
        return os;

    }

    ~Brick() = default;

    void setY(int y1) {
        Brick::y = y1;
    }


    void setHp(int hp1) {
        Brick::hp = hp1;
    }

    void setAlive(bool alive1) {
        Brick::alive = alive1;
    }

    void setX(int x1) {
        Brick::x = x1;
    }



    void setdetails(int x_, int y_, int hp_) {
        this->setX(x_);
        this->setY(y_);
        this->setHp(hp_);
        if(hp_>0)
            this->setAlive(true);
        else this->setAlive(false);
    }

    void damagebrick(){
        if(this->hp > 0)
            this->setHp(this->hp-1);
        if(this->hp == 0)
            this->setAlive(false);
    }



    //  mbr.update_matrix(br);

};

//
class Layer {
private:
    std::array<Brick,16> row;
public:
    Layer() {
        for(int i=0; i<16; ++i)
            row[i]=Brick();
    }
    explicit Layer(const std::array<Brick, 16> &row) : row(row) {}

    /*Layer& operator=(const Layer& other) {
        for(int i=0; i<rowsize; ++i)
            row[i]=other.row[i];
        return *this;
    }*/

    std::array<Brick, 16> &getRow()  {
        return row;
    }
};




class Wall {
private:
    std::array<Layer,6> wallmatrix;

public:
    Wall() {
        for(int i=0; i<6; ++i)
            wallmatrix[i]=Layer();
    }

    explicit Wall(const std::array<Layer, 6> &wallmatrix) : wallmatrix(wallmatrix) {}

    /*Wall& operator=(const Wall& other) {
        for(int i=0; i<6; ++i)
            wallmatrix[i]=other.wallmatrix[i];
        return *this;
    }*/

    std::array<Layer, 6> &getWallmatrix()  {
        return wallmatrix;
    }


};


class Game {
    Wall mbr;
    Line line;
    //Ball ball;
    std::array<std::array<int,16>,12> m{{{0}}};
public:
    [[nodiscard]] const std::array<std::array<int, 16>, 12> &getM() const {
        return m;
    }
    Game(Wall  mbr_, Line line_) : mbr{std::move(mbr_)}, line{line_} {}

    Game() = default;


    Wall &getMbr()  {
        return mbr;
    }

    Line &getLine() {
        return line;
    }




    void generatelevel(){
        std::ifstream fin("caramizi.txt");
        int n=0;
        char c;
        std::cout<<"Demonstratie generare nivel joc tip brick breaker:\nLegenda:\n'{}' - 3 hp\n'[]' - 2 hp\n'()' - 1 hp\n";
        std::cout<<"Datele nivelului sunt in fisierul caramizi.txt\n";
        fin>>n;
        std::cout<<"Doriti ca toate caramizile sa sufere 1 punct de damage?(y/n)\n";
        std::cin>>c;
        int i, len, x, y, hp;
        for(i = 1; i <= n; ++i){
            fin >> x;
            fin >> y;
            fin >> hp;
            this->getMbr().getWallmatrix()[x].getRow()[2*y].setdetails(x,y,hp);
            this->getMbr().getWallmatrix()[x].getRow()[2*y+1].setdetails(x,y,hp);
            if(c=='y') {
                this->getMbr().getWallmatrix()[x].getRow()[2 * y].damagebrick();
                this->getMbr().getWallmatrix()[x].getRow()[2 * y + 1].damagebrick();
            }
            m[x][2 * y] = this->getMbr().getWallmatrix()[x].getRow()[2*y].gethp();
            m[x][2 * y + 1] = this->getMbr().getWallmatrix()[x].getRow()[2*y].gethp();
        }
        x=this->getLine().getx();
        len=this->getLine().getlen();
        for(i = x; i <= x+len-1; i++)
            m[11][i] = -1;


    }



};



int main() {
    Game game;
    int i, j, br;
    game.generatelevel();
    for(i = 0; i < 11; i++) {
        for (j = 0; j < 15; j+=2) {
            br = game.getM()[i][j];
            if (br >= 3)
                std::cout << "{}";
            else if (br == 2)
                std::cout << "[]";
            else if (br == 1)
                std::cout << "()";
            else std::cout << "  ";
        }
        std::cout << "\n";
    }
    for(j = 0; j < 15; j++) {
        br = game.getM()[11][j];
        if (br == -1)
            std::cout << "-";
        else std::cout << " ";
    }

    return 0;
}