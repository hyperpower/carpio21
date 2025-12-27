#include <iostream>
#include <memory>
#include <string>
#include <list>
#include "geometry/geometry.hpp"


using namespace carpio;
typedef PointChain_<double, 2> PointChain;
typedef Point_<double, 2> Point;
typedef PolygonPartition_<Point> PP;

typedef GFile_<double,2> GF;

PointChain read(const std::string& filename) {
    PointChain chain = ReadFile<PointChain>(filename);
    chain.set_close();
    std::cout << "File = " << filename   << std::endl;
    std::cout << "size = " << chain.size() << std::endl;
    
    return chain;
}

void PlotPointChain(const PointChain& pc, std::string filename) {
    Gnuplot gnu;
    gnu.set_terminal_png(
            "./fig/" + filename//const std::string& filename,
           );
    auto a_arrows = ToGnuplotActorVector(pc);
    a_arrows.title(filename);
    a_arrows.style("with vectors lw 2 lc 2");
    gnu.add(a_arrows);
    gnu.plot();
}


void PlotPolygonPartitionInOrder(int count, 
        const std::string& filename,
        const PointChain& pc, 
        const PP::ListTri& triangles) {
    Gnuplot gnu;
    gnu.set_title("Ear Clipping Method");
    gnu.set_terminal_png("./fig/" + filename + "/" + tfm::format("%03d", count));
    
    auto a_arrows = ToGnuplotActorVector(pc);
    a_arrows.title(filename);
    a_arrows.style("with vectors lw 2 lc 2");
    gnu.add(a_arrows);

    gnu.set_xlabel("Step = " + std::to_string(count));

    int inner_c = 0;
    for(const auto& tri : triangles) {
        
        auto a_tri = ToGnuplotActor(tri, 0);
        if (inner_c < count){
            a_tri.style("with lines lw 2 lc 3");
            gnu.add(a_tri);
        }else if (inner_c == count) {
            a_tri.style("with lines lw 3 lc 1");
            gnu.add(a_tri);
        }else{
            break;
        }
        inner_c++;
    }
    gnu.plot();
}

int aCase(const std::string& filename) {
    std::cout << "Polygon Read : " << filename << std::endl;
    auto pc = read(filename + ".txt");
    PlotPointChain(pc, filename);
    std::cout << "Polygon Partition" << std::endl;
    PP pp;
    typename PP::ListTri res;
    pp.ear_clipping(pc, res);
    std::cout << "Number of Triangles: " << res.size() << std::endl;

    std::cout << "Make dir for output" << std::endl;
    MakeDir("./fig/" + filename);
    int count = 0;
    for (const auto& tri : res) {
        PlotPolygonPartitionInOrder(count, filename, pc, res);
        count++;
    }

    return 0;
}

// main 
int main(){
    std::vector<std::string> input_files = {
        "examplepolygon", "man", "shui"
    };

    for (const auto& filename : input_files) {
        aCase(filename);
    }

}

