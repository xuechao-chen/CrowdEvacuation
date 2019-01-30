#include "../RVO/RVO.h"
#include "../RVO/Agent.h"
#include <iostream>
#include "../CrowdRendering/CrowdRenderingInterface.h"

std::vector<RVO::Vector2> goals;

void setupScenario(RVO::RVOSimulator* sim) {
	// Specify global time step of the simulation.
	sim->setTimeStep(0.25f);

	// Specify default parameters for agents that are subsequently added.
	sim->setAgentDefaults(15.0f, 10, 10.0f, 5.0f, 2.0f, 2.0f);

	// Add agents, specifying their start position.
	for (size_t i = 0; i < 5; i++)
	{
		sim->addAgent(RVO::Vector2(10, 10+i*50));
	}
	
	// Create goals (simulator is unaware of these).
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		goals.push_back(RVO::Vector2(50, 50));
	}

	// Add (polygonal) obstacle(s), specifying vertices in counterclockwise order.
	/*std::vector<RVO::Vector2> vertices;
	vertices.push_back(RVO::Vector2(-7.0f, -20.0f));
	vertices.push_back(RVO::Vector2(7.0f, -20.0f));
	vertices.push_back(RVO::Vector2(7.0f, 20.0f));
	vertices.push_back(RVO::Vector2(-7.0f, 20.0f));
	sim->addObstacle(vertices);*/

	// Process obstacles so that they are accounted for in the simulation.
	sim->processObstacles();
}

void updateVisualization(RVO::RVOSimulator* sim) {
	// Output the current global time.
	//std::cout << sim->getGlobalTime() << " ";

	std::vector<glm::vec2> AgentPosSet;

	// Output the position for all the agents.
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		//std::cout << sim->getAgentPosition(i) << " ";
		const RVO::Vector2& Pos = sim->getAgentPosition(i);
		AgentPosSet.push_back({ Pos.x(), Pos.y() });
	}

	//std::cout << std::endl;
	hiveCrowdRendering::drawAgents(AgentPosSet);
	hiveCrowdRendering::display();
	hiveCrowdRendering::clear();
}

bool reachedGoal(RVO::RVOSimulator* sim) {
	// Check whether all agents have arrived at their goals.
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		//if (absSq(goals[i] - sim->getAgentPosition(i)) > sim->getAgentRadius(i) * sim->getAgentRadius(i)) {
		//	// Agent is further away from its goal than one radius.
		//	return false;
		//}
		auto Agent = sim->getAgent(i);
		if (Agent->isFinished(10.0f))
		{
			return false;
		}
	}
	return true;
}

void setPreferredVelocities(RVO::RVOSimulator* sim) {
	// Set the preferred velocity for each agent.
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		if (absSq(goals[i] - sim->getAgentPosition(i)) < sim->getAgentRadius(i) * sim->getAgentRadius(i)) {
			// Agent is within one radius of its goal, set preferred velocity to zero
			sim->setAgentPrefVelocity(i, RVO::Vector2(0.0f, 0.0f));
		}
		else {
			// Agent is far away from its goal, set preferred velocity as unit vector towards agent's goal.
			sim->setAgentPrefVelocity(i, normalize(goals[i] - sim->getAgentPosition(i)));
		}
	}
}

int main()
{
	hiveCrowdRendering::init("RenderingConfig.xml");

	// Create a new simulator instance.
	RVO::RVOSimulator* sim = new RVO::RVOSimulator();

	// Set up the scenario.
	setupScenario(sim);

	// Perform (and manipulate) the simulation.
	do {
		updateVisualization(sim);
		setPreferredVelocities(sim);
		sim->doStep();
	} while (!reachedGoal(sim));

	delete sim;
}
