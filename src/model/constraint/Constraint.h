#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINT_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINT_H_

/*!	\class Constraint
 * 	Virtual class inherited by all Constraints class
 */

class Constraint {

public:

	virtual int getPriority() const;
};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINT_H_ */
