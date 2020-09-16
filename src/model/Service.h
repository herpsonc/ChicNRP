#include <map>
#include <array>
#include <fstream>
#include <sstream>
#include "Agent.h"
#include "constraint/Constraint.h"
#include "PredefinedPlanning.h"


using namespace std;

#ifndef MODEL_SERVICE_H_
#define MODEL_SERVICE_H_

class Service {
public:
	Service();
	Service(string id);
	virtual ~Service();
	const string& getId() const;
	void setId(const string &id);

	const vector<Post*>& getPosts() const;
	void setPosts(const vector<Post*> &posts);
	void addPost(Post* post);
	Post* getPost(string id);

	const vector<Agent*>& getReferents() const;
	void setReferents(const vector<Agent*> &referents);
	void addReferent(Agent* referent);

	const vector<Constraint*>& getConstraints() const;
	void setConstraints(const vector<Constraint*> &constraints);
	void addConstraint(Constraint* constraint);

	const array<map<Post*, int>, 7>& getPostRequired() const;
	void setPostRequired(const array<map<Post*, int>, 7> &postRequired);
	void addPostRequired(Post* post, int nb);
	void addPostRequired(Post* post, int nb, int day);

	const vector<Agent*>& getAgents() const;
	void setAgents(const vector<Agent*> &agents);
	void addAgent(Agent* agent);

	void setPostRequirementPriority(int priority);
	int getPostRequirementPriority();

	void setPredefinedPlanning(PredefinedPlanning planning);
	PredefinedPlanning* getPredefinedPlanning();
	void loadPredefinedPlanning(string fileName, int nbDays, int firstDay);

protected:
	string id;
	vector<Post*> posts;
	vector<Agent*> referents; //Les référents ne sont pas forcément affectés à ce service
	vector<Agent*> agents; //Agents affectés au service
	array<map<Post*,int>, 7> postRequired; //tableau de 7 map, chaque map correspond a des couples Poste/nbRequis (C5)
	vector<Constraint*> constraints;
	int postRequirementPriority;
	PredefinedPlanning predefinedPlanning;
};

#endif /* MODEL_SERVICE_H_ */
