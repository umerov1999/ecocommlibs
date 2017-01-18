package text;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.text.LexicalUnit;
import common.CFG;

/**
 * The <code>CreateLexicalUnit</code> example code shows you the basic steps to
 * create a <code>LexicalUnit</code> resource, add regular expressions to it and
 * store it on the hard drive as a resource file.
 */
public class CreateLexicalUnit
{
  /**
   * Main entry point for the <code>CreateLexicalUnit</code> example program.
   * 
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args) throws IOException
  {
    System.out.println("1) creating a new Engine with your user certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();
    
    System.out.println("2) creating a new LexicalUnit");
    final LexicalUnit lexicalUnit = LexicalUnit.create(engine);
    System.out.println(" . LexicalUnit created successfully");
    System.out.println(" . right after its creation, the status of the LexicalUnit is:");
    System.out.println("   . is the LexicalUnit compiled = " + lexicalUnit.isCompiled());
    System.out.println();

    System.out.println("3) editing our LexicalUnit in order to model: ");
    System.out.println();
    System.out.println(" num_month = 0[1-9]|1[0-2]");
    System.out.println(" num_day = 0[1-9]|[1-2][0-9]|3[0-1]");
    System.out.println(" day_of_birth = {num_day}");
    System.out.println(" month_of_birth = {num_month}");
    System.out.println(" year_of_birth = [0-9][0-9]");
    System.out.println(" extra_number = [0-9]");
    System.out.println(" sep = [- /]?");
    System.out.println(" id_number = {day_of_birth}{month_of_birth}{year_of_birth}{sep}{extra_number}?");
    System.out.println(" {id_number}");
    System.out.println();
    
    lexicalUnit.defineRegex("num_month", "0[1-9]|1[0-2]");
    lexicalUnit.defineRegex("num_day", "0[1-9]|[1-2][0-9]|3[0-1]");
    lexicalUnit.defineRegex("day_of_birth", "{num_day}");
    lexicalUnit.defineRegex("month_of_birth", "{num_month}");
    lexicalUnit.defineRegex("year_of_birth", "[0-9][0-9]");
    lexicalUnit.defineRegex("extra_number", "[0-9]");
    lexicalUnit.defineRegex("sep", "[- /]?");
    lexicalUnit.defineRegex("id_number", "{day_of_birth}{month_of_birth}{year_of_birth}{sep}{extra_number}?");
    lexicalUnit.defineMainRegex("{id_number}");
    
    System.out.println(" . edited the LexicalUnit successfully");
    System.out.println();
    
    System.out.println("4) compiling the LexicalUnit (mandatory if you want to store and/or use it):");
    lexicalUnit.compile();
    System.out.println(" . LexicalUnit compiled successfully");
    System.out.println("   . is the LexicalUnit compiled = " + lexicalUnit.isCompiled());
    System.out.println();
    
    System.out.println("5) storing the LexicalUnit as a resource file:");
    lexicalUnit.store(CFG.OUTPUT_DIR + "/CreateLexicalUnit-output.res");
    System.out.println(" . " + new File(CFG.OUTPUT_DIR + "/CreateLexicalUnit-output.res").getCanonicalPath() + " stored successfully");
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("6) cleaning-up: disposing of objects and disposing of the Engine");
    lexicalUnit.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to create, edit, store and dispose of a LexicalUnit");
  }
  
} // CreateLexicalUnit
