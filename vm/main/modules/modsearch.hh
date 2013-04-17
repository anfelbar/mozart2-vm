#ifndef __MODSEARCH_H
#define __MODSEARCH_H

#include "../mozartcore.hh"
#include <gecode/search.hh>

namespace mozart {
namespace builtins {

///////////////////
// Search Module //
///////////////////

class ModSearch: public Module {
public:
	ModSearch(): Module("Search") {}

	class DFS: public Builtin<DFS> {
	public:
		DFS(): Builtin("dfs") {}

	    static void call(VM vm, In space) {
			if(ConstraintSpace(space).isConstraintSpace(vm)) { 
				GecodeSpace* gs = ConstraintSpace(space).constraintSpace(vm);
				Gecode::DFS<GecodeSpace> e(gs);
				while (GecodeSpace *sol = e.next())
					ConstraintSpace(space).changeConstraintSpace(vm, sol);

			}

		}

	};

};

} // namespace builtins
} // namespace mozart
#endif // __MODSEARCH_H


