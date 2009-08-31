// Begin CVS Header 
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/bindings/java/examples/example7.java,v $ 
//   $Revision: 1.1 $ 
//   $Name:  $ 
//   $Author: gauges $ 
//   $Date: 2009/08/31 19:35:07 $ 
// End CVS Header 

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., EML Research, gGmbH, University of Heidelberg, 
// and The University of Manchester. 
// All rights reserved. 

/**
 * This is an example on how to create user defined kinetic functions with the COPASI API
 */
import org.COPASI.*;

public class example7
{

 public static void main(String[] args)
 {
     assert CCopasiRootContainer.getRoot() != null;
     // create a new datamodel
     CCopasiDataModel dataModel = CCopasiRootContainer.addDatamodel();
     assert CCopasiRootContainer.getDatamodelList().size() == 1;
     // get the model from the datamodel
     CModel model = dataModel.getModel();
     assert model != null;
     // set the units for the model
     // we want seconds as the time unit
     // microliter as the volume units
     // and nanomole as the substance units
     model.setTimeUnit(CModel.s);
     model.setVolumeUnit(CModel.microl);
     model.setQuantityUnit(CModel.nMol);

     // we have to keep a set of all the initial values that are changed during
     // the model building process
     // They are needed after the model has been built to make sure all initial
     // values are set to the correct initial value
     ObjectStdVector changedObjects=new ObjectStdVector();

     // create a compartment with the name cell and an initial volume of 5.0
     // microliter
     CCompartment compartment = model.createCompartment("cell", 5.0);
     CCopasiObject object = compartment.getObject(new CCopasiObjectName("Reference=InitialVolume"));
     assert object != null;
     changedObjects.add(object);
     assert compartment != null;
     assert model.getCompartments().size() == 1;
     // create a new metabolite with the name S and an inital
     // concentration of 10 nanomol
     // the metabolite belongs to the compartment we created and is is to be
     // fixed
     CMetab S = model.createMetabolite("S", compartment.getObjectName(), 10.0, CMetab.FIXED);
     object = S.getObject(new CCopasiObjectName("Reference=InitialConcentration"));
     assert(object != NULL);
     changedObjects.add(object);
     assert(compartment != NULL);
     assert S != null;
     assert model.getMetabolites().size() == 1;
     // create a second metabolite called P with an initial
     // concentration of 0. This metabolite is to be changed by reactions
     CMetab* P = model.createMetabolite("P", compartment.getObjectName(), 0.0, CMetab.REACTIONS);
     assert P != null;
     object = P.getObject(new CCopasiObjectName("Reference=InitialConcentration"));
     assert object != null;
     changedObjects.add(object);
     assert model.getMetabolites().size() == 2;

     // now we create a reaction
     CReaction reaction = model.createReaction("reaction");
     assert reaction != null;
     assert model.getReactions().size() == 1;
     // reaction converts S to P
     // we can set these on the chemical equation of the reaction
     CChemEq chemEq = reaction.getChemEq();
     // S is a substrate with stoichiometry 1
     chemEq.addMetabolite(S.getKey(), 1.0, CChemEq.SUBSTRATE);
     // P is a product with stoichiometry 1
     chemEq.addMetabolite(P.getKey(), 1.0, CChemEq.PRODUCT);
     assert chemEq.getSubstrates().size() == 1;
     assert chemEq.getProducts().size() == 1;
     // this reaction is to be irreversible
     reaction.setReversible(false);
     assert reaction.isReversible() == false;

     CModelValue MV = model.createModelValue("K", 42.0);
     // set the status to FIXED
     MV.setStatus(CModelValue.FIXED);
     assert MV != null;
     object = MV.getObject(new CCopasiObjectName("Reference=InitialValue"));
     assert object != null;
     changedObjects.add(object);
     assert model.getModelValues().size() == 1;

     // now we ned to set a kinetic law on the reaction
     // for this we create a user defined function
     CFunctionDB funDB = CCopasiRootContainer.getFunctionList();
     assert funDB != null;

     CKinFunction function = new CKinFunction("My Rate Law");

     funDB.add(function, true);
     CFunction rateLaw = (CFunction*)(funDB.findFunction("My Rate Law"));

     assert rateLaw != null;

     // now we create the formula for the function and set it on the function
     String formula = "(1-0.4/(EXPONENTIALE^(temp-37)))*0.00001448471257*1.4^(temp-37)*substrate";

     boolean result = function.setInfix(formula);
     assert result == true;
     // make the function irreversible
     function.setReversible(COPASI.TriFalse);
     // the formula string should have been parsed now
     // and COPASI should have determined that the formula string contained 2 parameters (temp and substrate)
     CFunctionParameters variables = function.getVariables();
     // per default the usage of those parameters will be set to VARIABLE
     long index = function.getVariableIndex("temp");
     assert index != C_INVALID_INDEX;
     CFunctionParameter param = variables.get(index);
     assert param.getUsage() == CFunctionParameter.VARIABLE;
     // This is correct for temp, but substrate should get the usage SUBSTRATE in order
     // for us to use the function with the reaction created above
     // So we need to set the usage for "substrate" manually
     index = function.getVariableIndex("substrate");
     assert index != C_INVALID_INDEX;
     param = variables.get(index);
     param.setUsage(CFunctionParameter.SUBSTRATE);

     // set the rate law for the reaction
     reaction.setFunction(function);
     assert reaction.getFunction() != null;

     // COPASI also needs to know what object it has to assocuiate with the individual function parameters
     // In our case we need to tell COPASI that substrate is to be replaced by the substrate of the reaction
     // and temp is to be replaced by the global parameter K
     reaction.setParameterMapping("substrate", S.getKey());
     reaction.setParameterMapping("temp", MV.getKey());
     
     // finally compile the model
     // compile needs to be done before updating all initial values for
     // the model with the refresh sequence
     model.compileIfNecessary();

     // now that we are done building the model, we have to make sure all
     // initial values are updated according to their dependencies
     model.updateInitialValues(changedObjects);

     // save the model to a COPASI file
     // we save to a file named example1.cps 
     // and we want to overwrite any existing file with the same name
     // Default tasks are automatically generated and will always appear in cps
     // file unless they are explicitley deleted before saving.
     dataModel.saveModel("example7.cps", true);

     // export the model to an SBML file
     // we save to a file named example1.xml, we want to overwrite any
     // existing file with the same name and we want SBML L2V3
     try
     {
       dataModel.exportSBML("example7.xml", true, 2, 3);
     }
     catch(java.lang.Exception ex)
     {
        System.err.println("Error. Exporting the model to SBML failed.");
     }
 }
}
