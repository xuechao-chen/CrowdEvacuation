/*
 * Agent.h
 * RVO2 Library
 *
 * Copyright (c) 2008-2010 University of North Carolina at Chapel Hill.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and non-profit purposes, without
 * fee, and without a written agreement is hereby granted, provided that the
 * above copyright notice, this paragraph, and the following four paragraphs
 * appear in all copies.
 *
 * Permission to incorporate this software into commercial products may be
 * obtained by contacting the Office of Technology Development at the University
 * of North Carolina at Chapel Hill <otd@unc.edu>.
 *
 * This software program and documentation are copyrighted by the University of
 * North Carolina at Chapel Hill. The software program and documentation are
 * supplied "as is," without any accompanying services from the University of
 * North Carolina at Chapel Hill or the authors. The University of North
 * Carolina at Chapel Hill and the authors do not warrant that the operation of
 * the program will be uninterrupted or error-free. The end-user understands
 * that the program was developed for research purposes and is advised not to
 * rely exclusively on the program for any reason.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
 * AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
 * SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT
 * CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 * DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 * STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON
 * AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE
 * AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Please send all bug reports to <geom@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Jur van den Berg, Stephen J. Guy, Jamie Snape, Ming C. Lin, Dinesh Manocha
 * Dept. of Computer Science
 * 201 S. Columbia St.
 * Frederick P. Brooks, Jr. Computer Science Bldg.
 * Chapel Hill, N.C. 27599-3175
 * United States of America
 *
 * <http://gamma.cs.unc.edu/RVO2/>
 */

#ifndef RVO_AGENT_H_
#define RVO_AGENT_H_

/**
 * \file       Agent.h
 * \brief      Contains the Agent class.
 */

#include "Definitions.h"
#include "RVOSimulator.h"

namespace RVO 
{
	enum EGroupMemberState
	{
		GMS_WAITING_MODE = 0,
		GMS_WALKING_MODE
	};

	class Agent 
	{
	public:
		explicit Agent(RVOSimulator *sim);

		Agent(RVOSimulator *sim, const Vector2 &position);

		/**
		 * \brief      Constructs an agent instance with the specified
		 *             properties.
		 * \param      sim             The simulator instance.
		 * \param      position        The two-dimensional starting position of
		 *                             this agent.
		 * \param      neighborDist    The maximum neighbor distance of this agent.
		 * \param      maxNeighbors    The maximum neighbor count of this agent.
		 * \param      radius          The radius of this agent.
		 * \param      timeHorizon     The time horizon of this agent.
		 * \param      timeHorizonObst The time horizon of this agent with respect
		 *                             to static obstacles.
		 * \param      maxSpeed        The maximum speed of this agent.
		 * \param      velocity        The initial two-dimensional linear velocity
		 *                             of this agent.
		 */
		Agent(RVOSimulator *sim, const Vector2 &position, float neighborDist,
			  size_t maxNeighbors, float timeHorizon, float timeHorizonObst,
			  float radius, const Vector2 &velocity, float maxSpeed);

		/**
		 * \brief      Computes the neighbors of this agent.
		 */
		void computeNeighbors();

		/**
		 * \brief      Computes the new velocity of this agent.
		 */
		void computeNewVelocity();

		/**
		 * \brief      Inserts an agent neighbor into the set of neighbors of
		 *             this agent.
		 * \param      agentNo         The number of the agent to be inserted.
		 * \param      rangeSq         The squared range around this agent.
		 */
		void insertAgentNeighbor(size_t agentNo, float &rangeSq);

		/**
		 * \brief      Inserts a static obstacle neighbor into the set of neighbors
		 *             of this agent.
		 * \param      obstacleNo      The number of the static obstacle to be
		 *                             inserted.
		 * \param      rangeSq         The squared range around this agent.
		 */
		void insertObstacleNeighbor(size_t obstacleNo, float rangeSq);

		/**
		 * \brief      Updates the two-dimensional position and two-dimensional
		 *             velocity of this agent.
		 */
		void update();
		bool judgePointInRectangle(const RVO::Vector2& Point, const std::vector<RVO::Vector2>& Rectangle);

	public:
		void useDefaultParameter(float neighborDist = 15.0f, size_t maxNeighbors = 10, float timeHorizon = 10.f, float timeHorizonObst = 10.f, float radius = 1.5f, float maxSpeed = 2.0f, const Vector2 &velocity = Vector2());
		const Vector2& getPosition() const {return position_;}
		const Vector2& getGoalPos() {return goalPos;}
		const Vector2& getVelocity() {return velocity_;}
		float getViewDistance() const {return viewDistance;}
		float getRadius() const {return radius_;}
		float getDistanceToGoal() const {return distanceToGoal;}
		unsigned int getGroupID() const {return GroupID;}

		void setDistanceToGoal(float vDistance) {distanceToGoal = vDistance;}
		void setGoalPos(const Vector2 vGoalPos) {goalPos = vGoalPos;}
		void setViewDistance(float vDistance) {viewDistance = vDistance;}
		void setGroupID(unsigned int vID) {GroupID = vID;}
		void setPrefVelocity(const Vector2& Velocity) {prefVelocity_ = Velocity;}
		void setPosition(const Vector2& Position) {position_ = Position;}

		void changeToWaitingMode() {m_State = EGroupMemberState::GMS_WAITING_MODE;}
		void changeToWalkingMode() {m_State = EGroupMemberState::GMS_WALKING_MODE;}

		bool isWaitingState() {return m_State == EGroupMemberState::GMS_WAITING_MODE;}
		bool isWalkingState() {return m_State == EGroupMemberState::GMS_WALKING_MODE;}

		bool isFinished(float vRadiusOfGoal);
		void setFinishFlag() {m_IsReached = false;}
		void setSpeed(float vSpeed) {speed_ = vSpeed;}

	private:
		bool m_IsReached;
		std::vector<std::pair<float, size_t> > agentNeighbors_;
		size_t maxNeighbors_;
		float maxSpeed_;
		float neighborDist_;
		Vector2 newVelocity_;
		std::vector<std::pair<float, size_t> > obstacleNeighbors_;
		std::vector<Line> orcaLines_;
		Vector2 position_;
		Vector2 prefVelocity_;
		float radius_;
		RVOSimulator *sim_;
		float timeHorizon_;
		float timeHorizonObst_;
		float viewDistance;
		float distanceToGoal;
		Vector2 velocity_;
		Vector2 goalPos;
		unsigned int GroupID;
		float speed_;

		EGroupMemberState m_State;

		friend class KdTree;
		friend class RVOSimulator;
		friend class CSGNGroupMember;
		friend class CSGNGroup;
	};

	/**
	 * \relates    Agent
	 * \brief      Solves a one-dimensional linear program on a specified line
	 *             subject to linear constraints defined by lines and a circular
	 *             constraint.
	 * \param      lines         Lines defining the linear constraints.
	 * \param      lineNo        The specified line constraint.
	 * \param      radius        The radius of the circular constraint.
	 * \param      optVelocity   The optimization velocity.
	 * \param      directionOpt  True if the direction should be optimized.
	 * \param      result        A reference to the result of the linear program.
	 * \return     True if successful.
	 */
	bool linearProgram1(const std::vector<Line> &lines, size_t lineNo,
						float radius, const Vector2 &optVelocity,
						bool directionOpt, Vector2 &result);

	/**
	 * \relates    Agent
	 * \brief      Solves a two-dimensional linear program subject to linear
	 *             constraints defined by lines and a circular constraint.
	 * \param      lines         Lines defining the linear constraints.
	 * \param      num           Count of lines.
	 * \param      radius        The radius of the circular constraint.
	 * \param      optVelocity   The optimization velocity.
	 * \param      directionOpt  True if the direction should be optimized.
	 * \param      result        A reference to the result of the linear program.
	 * \return     True if successful.
	 */
	bool linearProgram2(const std::vector<Line> &lines, size_t num, float radius,
						const Vector2 &optVelocity, bool directionOpt,
						Vector2 &result);

	/**
	 * \relates    Agent
	 * \brief      Solves a two-dimensional linear program subject to linear
	 *             constraints defined by lines and a circular constraint.
	 * \param      lines         Lines defining the linear constraints.
	 * \param      numObstLines  Count of obstacle lines.
	 * \param      radius        The radius of the circular constraint.
	 * \param      result        A reference to the result of the linear program.
	 */
	void linearProgram3(const std::vector<Line> &lines, size_t numObstLines,
						float radius, Vector2 &result);
}

#endif /* RVO_AGENT_H_ */
