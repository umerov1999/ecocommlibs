package text;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.text.SubsetKnowledge;
import common.CFG;

/**
 * The <code>CreateSubsetKnowledge</code> example code shows you the basic steps
 * to create a <code>SubsetKnowledge</code> resource that constrains the
 * characters a handwriting recognizer is allowed to recognize.
 */
public class CreateSubsetKnowledge
{
  /**
   * Main entry point for the <code>CreateSubsetKnowledge</code> example
   * program.
   * 
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args) throws IOException
  {
    System.out.println("1) creating a new Engine with your user certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();
    
    System.out.println("2) creating a new SubsetKnowledge");
    final SubsetKnowledge sk = SubsetKnowledge.create(engine);
    System.out.println(" . SubsetKnowledge created successfully");
    System.out.println();

    System.out.println("3) disabling all characters in the SubsetKnowledge");
    sk.disableAllCharacters();
    System.out.println(" . all characters disabled successfully");
    System.out.println();
    
    System.out.println("4) enabling \"abcdefghijklmnopqrstuvwxyz\" characters");
    sk.enableCharacters("abcdefghijklmnopqrstuvwxyz");
    System.out.println(" . characters enabled successfully");
    System.out.println();
    
    System.out.println("5) storing the SubsetKnowledge as a resource file:");
    sk.store(CFG.OUTPUT_DIR + "/CreateSubsetKnowledge-output.res");
    System.out.println(" . " + new File(CFG.OUTPUT_DIR + "/CreateSubsetKnowledge-output.res").getCanonicalPath() + " stored successfully");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("6) cleaning-up: disposing of objects and disposing of the Engine");
    sk.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to create, edit, store and dispose of a SubsetKnowledge");
  }
  
} // CreateSubsetKnowledge
