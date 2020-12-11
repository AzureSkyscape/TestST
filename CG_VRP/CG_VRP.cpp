// -------------------------------------------------------------- -*- C++ -*-
// File: ilolpex1.cpp
// Version 12.10.0
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5725-A06 5725-A29 5724-Y48 5724-Y49 5724-Y54 5724-Y55 5655-Y21
// Copyright IBM Corporation 2000, 2019. All Rights Reserved.
//
// US Government Users Restricted Rights - Use, duplication or
// disclosure restricted by GSA ADP Schedule Contract with
// IBM Corp.
// --------------------------------------------------------------------------
//
// ilolpex1.cpp - Entering and optimizing a problem.  Demonstrates different
// methods for creating a problem.  The user has to choose the method
// on the command line:
//
//    ilolpex1  -r     generates the problem by adding rows
//    ilolpex1  -c     generates the problem by adding columns
//    ilolpex1  -n     generates the problem by adding a list of coefficients

#include <iostream>
#include <string>
#include <ilcplex/ilocplex.h>

using namespace std;

int main(int argc, char **argv) {
	IloEnv   env;
	try {
		IloModel model(env);

		IloNumVarArray var(env);
		IloRangeArray con(env);

		//populatebycolumn(model, var, con);
		IloEnv env = model.getEnv();

		IloObjective obj = IloMaximize(env);
		con.add(IloRange(env, -IloInfinity, 20.0, "c1"));
		con.add(IloRange(env, -IloInfinity, 60.0, "c2"));
		con.add(IloRange(env, -IloInfinity, 40.0, "c3"));

		var.add(IloNumVar(obj(1.0) + con[0](-1.0) + con[1](1.0) + con[2](1.0)));
		var.add(IloNumVar(obj(2.0) + con[0](1.0) + con[1](-3.0)));
		var.add(IloNumVar(obj(3.0) + con[0](1.0) + con[1](1.0)));

		var[0].setName("x1");
		var[1].setName("x2");
		var[2].setName("x3");

		model.add(obj);
		model.add(con);

		IloCplex cplex(model);
		cplex.exportModel("lpex1.lp");

		// Optimize the problem and obtain solution.
		if (!cplex.solve()) {
			std::cout << "Failed to optimize LP" << endl;
			throw(-1);
		}

		IloNumArray vals(env);
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value  = " << cplex.getObjValue() << endl;
		cplex.getValues(vals, var);
		env.out() << "Values        = " << vals << endl;
		cplex.getSlacks(vals, con);
		env.out() << "Slacks        = " << vals << endl;
		cplex.getDuals(vals, con);
		env.out() << "Duals         = " << vals << endl;
		cplex.getReducedCosts(vals, var);
		env.out() << "Reduced Costs = " << vals << endl;

		IloNumArray lower(env);
		IloNumArray upper(env);
		cplex.getObjSA(lower, upper, var);
		env.out() << "lows          = " << lower << endl;
		env.out() << "upper         = " << upper << endl;

		IloNumArray RHSlower(env);
		IloNumArray RHSupper(env);
		cplex.getRHSSA(RHSlower, RHSupper, con);
		env.out() << "RHSlower     = " << RHSlower << endl;
		env.out() << "RHSupper     = " << RHSupper << endl;
		cout << "End CPLEX" << endl;
		/*display sensitivity obj *;
		display sensitivity rhs;*/

	}
	catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}

	env.end();

	return 0;
}  // END main
