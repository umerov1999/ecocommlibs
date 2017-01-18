package text;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.text.Archive;
import com.myscript.text.Lexicon;
import com.myscript.text.SubsetKnowledge;
import common.CFG;

/**
 * The <code>CreateArchive</code> example code shows you the basic steps to
 * create an <code>Archive</code> resource, attach other resources to it and
 * then store it on the hard drive as a resource file.
 */
public class CreateArchive
{
  /**
   * Main entry point for the <code>CreateArchive</code> example program.
   * 
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args) throws IOException
  {
    System.out.println("1) creating a new Engine with your user certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());

    System.out.println(" . Engine created successfully");
    System.out.println();
    
    System.out.println("2) creating a new Archive");
    final Archive archive = Archive.create(engine);

    System.out.println(" . Archive created successfully");
    System.out.println(" . right after its creation, the status of the Archive is:");
    System.out.println("   . number of resources in the Archive = " + archive.getAttachedCount());
    System.out.println();

    System.out.println("3) creating a Lexicon and a SubsetKnowledge we will attach to the Archive");
    
    final Lexicon lexicon = Lexicon.create(engine);
    lexicon.addWord("this");
    lexicon.addWord("is");
    lexicon.addWord("a");
    lexicon.addWord("sample");
    lexicon.addWord("lexicon");
    lexicon.compile(); // compile before attaching
    
    System.out.println(" . Lexicon created, edited and compiled successfully");

    final SubsetKnowledge sk = SubsetKnowledge.create(engine);
    sk.disableAllCharacters();
    sk.enableCharacters("abcdefghijklmnopqrstuvwxyz"); // enable only lowercase letters
    System.out.println(" . SubsetKnowledge created and edited successfully");
    System.out.println();
    
    System.out.println("4) attaching the Lexicon and SubsetKnowledge to the Archive");
    System.out.println(" . attaching the Lexicon to the Archive");
    archive.attach(lexicon);
    System.out.println(" . Lexicon attached successfully");
    
    System.out.println(" . disposing of the Lexicon (not needed anymore because we attached it)");
    lexicon.dispose();
    System.out.println(" . Lexicon disposed of successfully");
    
    System.out.println(" . attaching the SubsetKnowledge to the Archive");
    archive.attach(sk);
    System.out.println(" . SubsetKnowledge attached successfully");
    
    System.out.println(" . disposing of the SubsetKnowledge (not needed anymore because we attached it)");
    sk.dispose();
    System.out.println(" . SubsetKnowledge disposed of successfully");
    
    System.out.println(" . right after having attached the resources, the status of the Archive is:");
    System.out.println("   . number of resources in the Archive = " + archive.getAttachedCount());
    System.out.println();

    System.out.println("5) storing the Archive as a resource file:");
    archive.store(CFG.OUTPUT_DIR + "/CreateArchive-output.res");
    System.out.println(" . " + new File(CFG.OUTPUT_DIR + "/CreateArchive-output.res").getCanonicalPath() + " stored successfully");
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("6) cleaning-up: disposing of objects and disposing of the Engine");
    archive.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to create, edit, store and dispose of an Archive");
  }
  
} // CreateArchive
