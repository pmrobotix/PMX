#include <cstdio>

#include "Simulator.hpp"


using namespace std;

int main(int, char** )
{

	Simulator &sim = Simulator::instance();

	sim.execute();

	return 0;
}
