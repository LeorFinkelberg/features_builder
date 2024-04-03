#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <array>
#include <numeric>
#include <math.h>
#include <limits>

#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include <scip/type_retcode.h>
#include <scip/struct_cons.h>
#include <scip/type_cons.h>
#include <scip/cons_linear.h>

#include <objscip/objscip.h>

using namespace std;


SCIP_RETCODE features(const char* lp_name, 
                      const char* features_csv, 
                      const char* features_cons_csv,
                      const char* scenario_name, 
                      bool useVarBinaryOriginal,
                      bool useVarTypeTrans,
                      bool useVarStatus,
                      bool useVarMayRoundUp,
                      bool useVarMayRoundDown,
                      bool useVarIsActive,
                      bool useVarIsDeletable,
                      bool useVarIsRemovable,
                      bool useVarObj,
                      bool useVarPseudoSol,
                      bool useNLocksDown,
                      bool useNLocksUp,
                      bool useIsTransformed,
                      bool useMultaggrConstant,
                      bool useVarAggrScalar,
                      bool useVarAggrConstant,
                      bool useVarMultaggrNVars,
                      bool useVarBestBound,
                      bool useVarWorstBound,
                      bool useVarBranchFactor,
                      bool useVarBranchPriority,
                      bool useVarBranchDirection,
                      bool useVarNImpls0,
                      bool useVarNImpls1,
                      bool useVarGetNCliques0,
                      bool useVarGetNCliques1,
                      bool useVarConflictScore,
                      bool useVarAvgInferenceScore
    )
//SCIP_RETCODE features(int argc, const char** argv)
{
    SCIP* scip = nullptr;

    SCIPcreate(&scip);
    SCIPincludeDefaultPlugins(scip);

    SCIPreadProb(scip, lp_name, nullptr);

    int nvars = SCIPgetNVars(scip);
    SCIP_VAR** vars = SCIPgetVars(scip);

    SCIP_LINCONSSTATS linconsstats;
    SCIP_LINCONSSTATS* linconsstatsptr;
    linconsstatsptr = &linconsstats;

    SCIPclassifyConstraintTypesLinear(scip, linconsstatsptr);

    SCIP_CONS** conss = SCIPgetConss(scip);
    int nconss = SCIPgetNConss(scip);

    std::ofstream varType;
    varType.open("varType.txt");
    std:vector <bool> binVarQ(nvars, 0);
    for (int i = 0; i < nvars; i++) {
        if (SCIPvarIsBinary(vars[i]) || SCIPvarIsIntegral(vars[i])) {
            varType << SCIPvarGetName(vars[i]) << "\n";
        }
        if (SCIPvarIsBinary(vars[i])) {
            binVarQ[i] = 1;
        }
        else {
            binVarQ[i] = 0;
        }
    }

    const char* sep = ",";

    SCIPpresolve(scip);

    std::ofstream myfile;
    myfile.open(features_csv);
    myfile << "var" << sep << "scenario" << sep;
    if (useVarBinaryOriginal)
        myfile << "varBinaryOriginal" << sep;
    if (useVarTypeTrans)
        myfile << "varTypeTrans" << sep;
    if (useVarStatus)
        myfile << "varStatus" << sep;
    if (useVarMayRoundUp)
        myfile << "varMayRoundUp" << sep;
    if (useVarMayRoundDown)
        myfile << "varMayRoundDown" << sep;
    if (useVarIsActive)
        myfile << "varMayIsActive" << sep;
    if (useVarIsDeletable)
        myfile << "varIsDeletable" << sep;
    if (useVarIsRemovable)
        myfile << "varIsRemovable" << sep;
    if (useVarObj)
        myfile << "varObj" << sep;
    if (useVarPseudoSol)
        myfile << "varPseudoSol" << sep;
    if (useNLocksDown)
        myfile << "NLocksDown" << sep;
    if (useNLocksUp)
        myfile << "NLocksUp" << sep;
    if (useIsTransformed)
        myfile << "IsTransformed" << sep;
    if (useMultaggrConstant)
        myfile << "multaggrConstant" << sep;
    if (useVarAggrScalar)
        myfile << "varAggrScalar" << sep;
    if (useVarAggrConstant)
        myfile << "varAggrConstant" << sep;
    if (useVarMultaggrNVars)
        myfile << "varMultaggrNVars" << sep;
    if (useVarBestBound)
        myfile << "varBestBound" << sep;
    if (useVarWorstBound)
        myfile << "varWorstBound" << sep;
    if (useVarBranchFactor)
        myfile << "varBranchFactor" << sep;
    if (useVarBranchPriority)
        myfile << "varBranchPriority" << sep;
    if (useVarBranchDirection)
        myfile << "varBranchDirection" << sep;
    if (useVarNImpls0)
        myfile << "varNImpls0" << sep;
    if (useVarNImpls1)
        myfile << "varNImpls1" << sep;
    if (useVarGetNCliques0)
        myfile << "varGetNCliques0" << sep;
    if (useVarGetNCliques1)
        myfile << "varGetNCliques1" << sep;
    if (useVarConflictScore)
        myfile << "varConflictScore" << sep;
    if (useVarAvgInferenceScore)
        myfile << "varAvgInferenceScore\n";


    for (int i = 0; i < nvars; i++) {
        if (SCIPvarIsBinary(vars[i]) || SCIPvarIsIntegral(vars[i])) { 
            SCIP_VAR* var_tr = SCIPvarGetTransVar(vars[i]);
            myfile << SCIPvarGetName(vars[i]) << sep << scenario_name << sep;
                if (useVarBinaryOriginal)
                    myfile << binVarQ[i] << sep;
                if (useVarTypeTrans)
                    myfile << SCIPvarGetType(var_tr) << sep;
                if (useVarStatus)
                    myfile << SCIPvarGetStatus(var_tr) << sep;
                if (useVarMayRoundUp)
                    myfile << SCIPvarMayRoundUp(var_tr) << sep;
                if (useVarMayRoundDown)
                    myfile << SCIPvarMayRoundDown(var_tr) << sep;
                if (useVarIsActive)
                    myfile << SCIPvarIsActive(var_tr) << sep;
                if (useVarIsDeletable)
                    myfile << SCIPvarIsDeletable(var_tr) << sep;
                if (useVarIsRemovable)
                    myfile << SCIPvarIsRemovable(var_tr) << sep;
                if (useVarObj)
                    myfile << SCIPvarGetObj(var_tr) << sep;
                if (useVarPseudoSol)
                    myfile << SCIPvarGetPseudoSol(var_tr) << sep;
                if (useNLocksDown)
                    myfile << SCIPvarGetNLocksDown(var_tr) << sep;
                if (useNLocksUp)
                    myfile << SCIPvarGetNLocksUp(var_tr) << sep;
                if (useIsTransformed)
                    myfile << SCIPvarIsTransformedOrigvar(var_tr) << sep;
                if (useMultaggrConstant)
                    myfile << SCIPvarGetMultaggrConstant(var_tr) << sep;
                if (useVarAggrScalar)
                    myfile << SCIPvarGetAggrScalar(var_tr) << sep;
                if (useVarAggrConstant)
                    myfile << SCIPvarGetAggrConstant(var_tr) << sep;
                if (useVarMultaggrNVars)
                    myfile << SCIPvarGetMultaggrNVars(var_tr) << sep;
                if (useVarBestBound)
                    myfile << SCIPvarGetBestBoundGlobal(var_tr) << sep;
                if (useVarWorstBound)
                    myfile << SCIPvarGetWorstBoundGlobal(var_tr) << sep;
                if (useVarBranchFactor)
                    myfile << SCIPvarGetBranchFactor(var_tr) << sep;
                if (useVarBranchPriority)
                    myfile << SCIPvarGetBranchPriority(var_tr) << sep;
                if (useVarBranchDirection)
                    myfile << SCIPvarGetBranchDirection(var_tr) << sep;
                if (useVarNImpls0)
                    myfile << SCIPvarGetNImpls(var_tr, FALSE) << sep;
                if (useVarNImpls1)
                    myfile << SCIPvarGetNImpls(var_tr, TRUE) << sep;
                if (useVarGetNCliques0)
                    myfile << SCIPvarGetNCliques(var_tr, FALSE) << sep;
                if (useVarGetNCliques1)
                    myfile << SCIPvarGetNCliques(var_tr, TRUE) << sep;
                if (useVarConflictScore)
                    myfile << SCIPgetVarConflictScore(scip, var_tr) << sep;
                if (useVarAvgInferenceScore)
                    myfile << SCIPgetVarAvgInferenceScore(scip, var_tr) << "\n";
        }
    }

    myfile.close();

    SCIPfree(&scip);

    return SCIP_OKAY;
}

SCIP_RETCODE relaxFeatures(const char* rel_lp_name, 
                           const char* rel_features_csv, 
                           const char* scenario_name,
                           bool useRelaxSolVal,
                           bool useVarImplRedcost0,
                           bool useVarImplRedcost1,
                           bool useVarPseudocostScore,
                           bool useVarEqualToLb,
                           bool useVarEqualToUb)
{
    SCIP* scip = nullptr;

    SCIP_CALL(SCIPcreate(&scip));
    SCIP_CALL(SCIPincludeDefaultPlugins(scip));

    SCIP_CALL((SCIPreadProb(scip, rel_lp_name, nullptr)));
    //SCIP_CALL((SCIPreadParams(scip, "scip.set")));

    int nvars = SCIPgetNVars(scip);
    SCIP_VAR** vars = SCIPgetVars(scip);

    SCIP_CALL(SCIPsolve(scip));

    SCIP_SOL* rel_sol = SCIPgetBestSol(scip);

    std::set<std::string> fl;
    std::ifstream fin(R"(varType.txt)");
    std::string str;
    if (fin.is_open()) {
        while (fin >> str)
            fl.insert(str);
        fin.close();
    }
    remove("varType.txt");
    
    std::ofstream relax_features;
    const char* sep = ",";
    relax_features.open(rel_features_csv);
    relax_features << "var_rel" << sep;
        if (useRelaxSolVal)
            relax_features << "relaxSolVal" << sep;
        if (useVarImplRedcost0)
            relax_features << "varImplRedcost0" << sep;
        if (useVarImplRedcost1)
            relax_features << "varImplRedcost1" << sep;
        if (useVarPseudocostScore)
            relax_features << "varPseudocostScore" << sep;
        if (useVarEqualToLb)
            relax_features << "equalToLb" << sep;
        if (useVarEqualToUb)
            relax_features << "equalToUb\n";

    for (int i = 0; i < nvars; i++) {
        if (fl.find(SCIPvarGetName(vars[i])) != fl.end()) {
            relax_features << SCIPvarGetName(vars[i]) << sep;
                if (useRelaxSolVal)
                    relax_features << SCIPgetSolVal(scip, rel_sol, vars[i]) << sep;
                if (useVarImplRedcost0)
                    relax_features << SCIPgetVarImplRedcost(scip, vars[i], FALSE) << sep;
                if (useVarImplRedcost1)
                    relax_features << SCIPgetVarImplRedcost(scip, vars[i], TRUE) << sep;
                if (useVarPseudocostScore)
                    relax_features << SCIPgetVarPseudocostScore(scip, vars[i], SCIPgetSolVal(scip, rel_sol, vars[i])) << sep;
                if (useVarEqualToLb)
                    relax_features << SCIPisFeasEQ(scip, SCIPgetSolVal(scip, rel_sol, vars[i]), SCIPvarGetLbGlobal(vars[i])) << sep;
                if (useVarEqualToUb)
                    relax_features << SCIPisFeasEQ(scip, SCIPgetSolVal(scip, rel_sol, vars[i]), SCIPvarGetUbGlobal(vars[i])) << "\n";
        }

    }
    relax_features.close();

    SCIP_CALL(SCIPfree(&scip));
    return SCIP_OKAY;
}

SCIP_RETCODE solFileToCSV(const char* lp_name, const char* sol_file,
    const char* sol_file_csv_name, const char* scenario_name) {

    SCIP* scip = nullptr;

    SCIP_CALL(SCIPcreate(&scip));
    SCIP_CALL(SCIPincludeDefaultPlugins(scip));

    SCIP_CALL((SCIPreadProb(scip, lp_name, nullptr)));
    //SCIP_CALL((SCIPreadParams(scip, "scip.set")));

    int nvars = SCIPgetNVars(scip);
    SCIP_VAR** vars = SCIPgetVars(scip);

    SCIPreadSol(scip, sol_file);

    SCIP_SOL* sol = SCIPgetBestSol(scip);

    std::ofstream sol_csv;
    const char* sep = ",";
    sol_csv.open(sol_file_csv_name);
    sol_csv << "var_target" << sep << "target\n";
    for (int i = 0; i < nvars; i++) {
        if (SCIPvarIsBinary(vars[i]) || SCIPvarIsIntegral(vars[i])){
            sol_csv << SCIPvarGetName(vars[i]) << sep << 
                !SCIPisFeasZero(scip, SCIPgetSolVal(scip, sol, vars[i])) << "\n";
        }
    }
    sol_csv.close();

    SCIP_CALL(SCIPfree(&scip));

    return SCIP_OKAY;
}

int main(int argnum, char** argv){

    int i;
    std::cout<<"Feature builder"<<std::endl;
    bool useOptimalSol = char(argv[1][0]) == '1';

    const char* sol_name;
    if (useOptimalSol) {
        sol_name = argv[2];
        i = 2;
    }
    else {
        sol_name = "";
        i = 1;
    }

    const char* lp_name = argv[i + 1];
    const char* rel_lp_name = argv[i + 2];
    const char* scenario_name = argv[i + 3];
    const char* output_dir = argv[i + 4];
    printf(lp_name);
    printf("\n");
    printf(rel_lp_name);
    printf("\n");
    printf(scenario_name);
    printf("\n");
    printf(output_dir);
    printf("\n");

    bool useVarBinaryOriginal = char(argv[i + 5][0]) == '1';
    bool useVarTypeTrans = char(argv[i + 6][0]) == '1';
    bool useVarStatus = char(argv[i + 7][0]) == '1'; 
    bool useVarMayRoundUp = char(argv[i + 8][0]) == '1';
    bool useVarMayRoundDown = char(argv[i + 9][0]) == '1';
    bool useVarIsActive = char(argv[i + 10][0]) == '1';
    bool useVarIsDeletable = char(argv[i + 11][0]) == '1';
    bool useVarIsRemovable = char(argv[i + 12][0]) == '1';
    bool useVarObj = char(argv[i + 13][0]) == '1';
    bool useVarPseudoSol = char(argv[i + 14][0]) == '1';
    bool useNLocksDown = char(argv[i + 15][0]) == '1';
    bool useNLocksUp = char(argv[i + 16][0]) == '1';;
    bool useIsTransformed = char(argv[i + 17][0]) == '1';
    bool useMultaggrConstant = char(argv[i + 18][0]) == '1';
    bool useVarAggrScalar = char(argv[i + 19][0]) == '1';
    bool useVarAggrConstant = char(argv[i + 20][0]) == '1';
    bool useVarMultaggrNVars = char(argv[i + 21][0]) == '1';
    bool useVarBestBound = char(argv[i + 22][0]) == '1';
    bool useVarWorstBound = char(argv[i + 23][0]) == '1';
    bool useVarBranchFactor = char(argv[i + 24][0]) == '1';
    bool useVarBranchPriority = char(argv[i + 25][0]) == '1';
    bool useVarBranchDirection = char(argv[i + 26][0]) == '1';
    bool useVarNImpls0 = char(argv[i + 27][0]) == '1';
    bool useVarNImpls1 = char(argv[i + 28][0]) == '1';
    bool useVarGetNCliques0 = char(argv[i + 29][0]) == '1';
    bool useVarGetNCliques1 = char(argv[i + 30][0]) == '1';
    bool useVarConflictScore = char(argv[i + 31][0]) == '1';
    bool useVarAvgInferenceScore = char(argv[i + 32][0]) == '1';

    bool useRelaxSolVal = char(argv[i + 33][0]) == '1';

    bool useVarImplRedcost0 = char(argv[i + 34][0]) == '1';
    bool useVarImplRedcost1 = char(argv[i + 35][0]) == '1';
    bool useVarPseudocostScore = char(argv[i + 36][0]) == '1';
    bool useVarEqualToLb = char(argv[i + 37][0]) == '1';
    bool useVarEqualToUb = char(argv[i + 38][0]) == '1';

    char features_csv[255] = "";
    strcat_s(features_csv, output_dir);
    strcat_s(features_csv, "\\features_");
    strcat_s(features_csv, scenario_name);
    strcat_s(features_csv, ".csv");

    char features_cons_csv[255] = "";
    strcat_s(features_cons_csv, output_dir);
    strcat_s(features_cons_csv, "\\features_con_");
    strcat_s(features_cons_csv, scenario_name);
    strcat_s(features_cons_csv, ".csv");


    char rel_features_csv[255] = "";
    strcat_s(rel_features_csv, output_dir);
    strcat_s(rel_features_csv, "\\rel_features_");
    strcat_s(rel_features_csv, scenario_name);
    strcat_s(rel_features_csv, ".csv");

    char sol_file_csv_name[255] = "";
    strcat_s(sol_file_csv_name, output_dir);
    strcat_s(sol_file_csv_name, "\\target_");
    strcat_s(sol_file_csv_name, scenario_name);
    strcat_s(sol_file_csv_name, ".csv");

    features(lp_name, 
             features_csv, 
             features_cons_csv, 
             scenario_name,
             useVarBinaryOriginal,
             useVarTypeTrans,
             useVarStatus,
             useVarMayRoundUp,
             useVarMayRoundDown,
             useVarIsActive,
             useVarIsDeletable,
             useVarIsRemovable,
             useVarObj,
             useVarPseudoSol,
             useNLocksDown,
             useNLocksUp,
             useIsTransformed,
             useMultaggrConstant,
             useVarAggrScalar,
             useVarAggrConstant,
             useVarMultaggrNVars,
             useVarBestBound,
             useVarWorstBound,
             useVarBranchFactor,
             useVarBranchPriority,
             useVarBranchDirection,
             useVarNImpls0,
             useVarNImpls1,
             useVarGetNCliques0,
             useVarGetNCliques1,
             useVarConflictScore,
             useVarAvgInferenceScore
            );

    relaxFeatures(rel_lp_name, 
                  rel_features_csv, 
                  scenario_name,
                  useRelaxSolVal,
                  useVarImplRedcost0,
                  useVarImplRedcost1,
                  useVarPseudocostScore,
                  useVarEqualToLb,
                  useVarEqualToUb);

    if (useOptimalSol)
        solFileToCSV(lp_name, sol_name, sol_file_csv_name, scenario_name);
 
    return 0;
}
