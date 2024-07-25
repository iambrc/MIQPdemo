# To build the Gurobi C++ library, open a Developer Command Prompt for
# Visual Studio, change to this directory, and type 'nmake'

all: gurobi_c++mt.lib

CPP      = cl
CPPFLAGS = -DWIN64 -EHsc -MT -nologo
SRC_DIR  = ..\cpp

CPPMTOBJS  = Env.obj Model.obj attrprivate.obj Var.obj Constr.obj \
             LinExpr.obj QuadExpr.obj Exception.obj \
             Callback.obj Column.obj SOS.obj QConstr.obj \
             GenConstr.obj TempConstr.obj Batch.obj

{$(SRC_DIR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -I../../include -c "$<" -Fo"$@"

gurobi_c++mt.lib: $(CPPMTOBJS)
	lib $(CPPMTOBJS) /OUT:gurobi_c++mt.lib

clean:
	del *.obj gurobi_c++mt.lib
