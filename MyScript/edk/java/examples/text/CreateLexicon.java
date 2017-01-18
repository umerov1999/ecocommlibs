package text;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.text.Lexicon;
import common.CFG;

/**
 * The <code>CreateLexicon</code> example code shows you the basic steps to
 * create a <code>Lexicon</code> resource, add words to it and store it on the
 * hard drive as a resource file.
 */
public class CreateLexicon
{
  /**
   * Main entry point for the <code>CreateLexicon</code> example program.
   * 
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args) throws IOException
  {
    System.out.println("1) creating a new Engine with your user certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();
    
    System.out.println("2) creating a new Lexicon");
    final Lexicon lexicon = Lexicon.create(engine);
    System.out.println(" . Lexicon created successfully");
    System.out.println(" . right after its creation, the status of the Lexicon is:");
    System.out.println("   . is the Lexicon compiled = " + lexicon.isCompiled());
    System.out.println("   . number of words in the Lexicon = " + lexicon.getWordCount());
    System.out.println("   . length of the longest word in the Lexicon = " + lexicon.getMaxWordLength());
    System.out.println();

    System.out.println("3) adding 3 words to our Lexicon:");
    System.out.println(" . adding \"Ishtar\", \"Marduk\", \"Shamash\", names of Babylonian gods");
    lexicon.addWord("Ishtar");
    lexicon.addWord("Marduk");
    lexicon.addWord("Shamash");
    
    System.out.println(" . right after having added words, the status of the Lexicon is:");
    System.out.println("   . number of words in the Lexicon = " + lexicon.getWordCount());
    System.out.println("   . length of the longest word in the Lexicon = " + lexicon.getMaxWordLength());
    System.out.println();

    System.out.println("4) compiling the Lexicon object (mandatory if you want to store and/or use it):");
    lexicon.compile();
    System.out.println(" . Lexicon compiled successfully");
    System.out.println("   . is the Lexicon compiled = " + lexicon.isCompiled());
    System.out.println();
    
    System.out.println("5) storing the Lexicon object as a resource file:");
    lexicon.store(CFG.OUTPUT_DIR + "/CreateLexicon-output.res");
    System.out.println(" . " + new File(CFG.OUTPUT_DIR + "/CreateLexicon-output.res").getCanonicalPath() + " stored successfully");
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("6) cleaning-up: disposing of objects and disposing of the Engine");
    lexicon.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to create, edit, store and dispose of a Lexicon");
  }
  
} // CreateLexicon
