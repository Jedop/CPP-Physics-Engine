#pragma once
#include "Object.hpp"
#include <SFML/Graphics.hpp>

struct Node {
    bool isLeaf = true;
    sf::Vector2f position;
    float w;
    float h;
    int capacity = 16;
    std::vector<int> data;
    const std::vector<Object>* objects;
    int depth = 0;
    int children[4];

};

class QuadTree {
private:
    std::vector<Node> pool;
    int maxObjectsPerNode = 32;
    int maxDepth = 56;
    int nextFreeNode = 0;

    int allocateNode(float x, float y, float w, float h, const std::vector<Object>* objects){
        int idx = nextFreeNode;
        nextFreeNode++;

        assert(nextFreeNode <= pool.size());

        reset(idx);
        pool[idx].position = {x, y};
        pool[idx].w = w;
        pool[idx].h = h;
        pool[idx].objects = objects;
        return idx;
    }
public:
    QuadTree() {
        pool.resize(100000); 
    }
    void clear() {
        nextFreeNode = 0; 
    }
    void build(const std::vector<Object>& objects) {
        clear();
        allocateNode(1920/2, 1080/2, 1920, 1080, &objects);
        for (int i = 0; i < objects.size(); i++) {
            insert(0, i); 
        }
    }
    void insert(int node_idx, int obj_idx){
        if (!pool[node_idx].isLeaf) {
            distributeToChildren(node_idx, obj_idx);
            return;
        }

        pool[node_idx].data.push_back(obj_idx);

        if (pool[node_idx].data.size() > pool[node_idx].capacity && pool[node_idx].depth < maxDepth) {
            split(node_idx);

            for (int i: pool[node_idx].data){
                distributeToChildren(node_idx, i);
            }

            pool[node_idx].data.clear();
        }
    }
    void query(int node_idx, sf::FloatRect range, std::vector<int>& found){
        sf::FloatRect myRect({pool[node_idx].position.x - pool[node_idx].w/2, pool[node_idx].position.y - pool[node_idx].h/2} , {pool[node_idx].w, pool[node_idx].h});
        if (!myRect.findIntersection(range)) return;

        for (int idx : pool[node_idx].data) {
            found.push_back(idx);
        }

        if (!pool[node_idx].isLeaf) {
            query(pool[node_idx].children[0], range, found);
            query(pool[node_idx].children[1], range, found);
            query(pool[node_idx].children[2], range, found);
            query(pool[node_idx].children[3], range, found);
        }
    }
    void draw(int node_idx, sf::RenderWindow& window){
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(pool[node_idx].w, pool[node_idx].h));
        rect.setOrigin({pool[node_idx].w/2, pool[node_idx].h/2});
        rect.setPosition(pool[node_idx].position);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setOutlineThickness(1.0f);
        window.draw(rect);

        if (!pool[node_idx].isLeaf) {
            draw(pool[node_idx].children[0], window);
            draw(pool[node_idx].children[1], window);
            draw(pool[node_idx].children[2], window);
            draw(pool[node_idx].children[3], window);
        }
    }
private:
    void reset(int node_idx) {
        pool[node_idx].isLeaf = true;
        pool[node_idx].data.clear();
        for (int& c : pool[node_idx].children)
            c = -1;
    }
    void split(int node_idx) {
        pool[node_idx].isLeaf = false;
        float child_w = pool[node_idx].w / 2;
        float child_h = pool[node_idx].h / 2;
        float offX = child_w / 2;
        float offY = child_h / 2;

        int TopLeft     = allocateNode(pool[node_idx].position.x - offX, pool[node_idx].position.y - offY, child_w, child_h, pool[node_idx].objects);
        int TopRight    = allocateNode(pool[node_idx].position.x + offX, pool[node_idx].position.y - offY, child_w, child_h, pool[node_idx].objects);
        int BottomLeft  = allocateNode(pool[node_idx].position.x - offX, pool[node_idx].position.y + offY, child_w, child_h, pool[node_idx].objects);
        int BottomRight = allocateNode(pool[node_idx].position.x + offX, pool[node_idx].position.y + offY, child_w, child_h, pool[node_idx].objects);

        pool[TopLeft].depth     = pool[node_idx].depth + 1;
        pool[TopRight].depth    = pool[node_idx].depth + 1;
        pool[BottomLeft].depth  = pool[node_idx].depth + 1;
        pool[BottomRight].depth = pool[node_idx].depth + 1;

        pool[node_idx].children[0] = TopLeft;
        pool[node_idx].children[1] = TopRight;
        pool[node_idx].children[2] = BottomLeft;
        pool[node_idx].children[3] = BottomRight;
    }
    void distributeToChildren(int node_idx, int obj_idx){
        sf::Vector2f objPos = (*pool[node_idx].objects)[obj_idx].position;
        
        bool inTop = objPos.y < pool[node_idx].position.y;
        bool inBottom = objPos.y >= pool[node_idx].position.y; 
        bool inLeft = objPos.x < pool[node_idx].position.x;
        bool inRight = objPos.x >= pool[node_idx].position.x;

        if (inTop && inLeft)     insert(pool[node_idx].children[0], obj_idx);
        if (inTop && inRight)    insert(pool[node_idx].children[1], obj_idx);
        if (inBottom && inLeft)  insert(pool[node_idx].children[2], obj_idx);
        if (inBottom && inRight) insert(pool[node_idx].children[3], obj_idx);
    }
};