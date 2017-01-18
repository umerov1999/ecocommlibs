package math;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.edk.java.math.ParseTreeToLaTeX;
import com.myscript.edk.java.math.ParseTreeToMathML;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.math.MathAlphabetKnowledge;
import com.myscript.math.MathDocument;
import com.myscript.math.MathGrammar;
import com.myscript.math.MathNode;
import com.myscript.math.MathRecognizer;
import common.CFG;

/**
 * The <code>MathRecognition</code> example code shows you how to perform
 * recognition on a handwritten math.
 */
public class MathRecognition
{
  /**
   * Main entry point for the <code>MathRecognition</code> example program.
   */
  public static final void main(String[] args) throws IOException
  {
    if (args.length == 0)
    {
      args = new String[1];
      args[0] = CFG.DATA_DIR + "/math.ink";
    }
    
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new MathRecognizer");
    final MathRecognizer recognizer = MathRecognizer.create(engine);
    System.out.println(" . MathRecognizer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resources to the recognizer");
    final MathAlphabetKnowledge ak = (MathAlphabetKnowledge)EngineObject.load(engine, CFG.getMathAkRes());
    System.out.println(" . MathAlphabetKnowledge resource loaded successfully");
    recognizer.attach(ak);
    System.out.println(" . MathAlphabetKnowledge resource attached successfully");
    System.out.println(" . disposing of the MathAlphabetKnowledge resource (not needed anymore)");
    ak.dispose();
    System.out.println(" . MathAlphabetKnowledge resource disposed of successfully");
    
    final MathGrammar grm = (MathGrammar)EngineObject.load(engine, CFG.getMathGrmRes());
    System.out.println(" . MathGrammar resource loaded successfully");
    recognizer.attach(grm);
    System.out.println(" . MathGrammar resource attached successfully");
    System.out.println(" . disposing of the MathGrammar resource (not needed anymore)");
    grm.dispose();
    System.out.println(" . MathGrammar resource disposed of successfully");
    System.out.println();

    System.out.println("4) creating a new MathDocument");
    final MathDocument math = MathDocument.create(engine);
    System.out.println(" . MathDocument created successfully");
    System.out.println();
    
    System.out.println("5) processing input files");
    
    for (int i = 0; i < args.length; ++i)
    {
      if (!new File(args[i]).exists())
      {
        System.err.println("missing ink file: " + args[i]);
        System.exit(-1);
      }
      
      System.out.println(" . parsing: " + new File(args[i]).getCanonicalPath());
      parseInput(args[i], math);
      System.out.println("   . input parsed successfully");

      System.out.println(" . running the math recognition process");
      recognizer.process(math);
      System.out.println("   . math recognition performed successfully");
      
      System.out.println(" . retrieving the recognition parse tree");
      if (math.hasParseTree())
      {
        final MathNode root = math.getParseTree();
        System.out.println("   . parse tree retrieved successfully");
        System.out.println();
        
        System.out.println(" . exploring the recognition parse tree");
        System.out.println("   . math recognized as LaTeX string: ");
        System.out.println();
        System.out.println(ParseTreeToLaTeX.format(root));
        System.out.println();

        System.out.println("   . math recognized as MathML string: ");
        System.out.println();
        System.out.println(ParseTreeToMathML.format(root));
        System.out.println();
        
        root.dispose();
      }
      else
      {
        System.out.println("   . no recognition result");
        System.out.println();
      }
      
      math.clear();
    }
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("6) cleaning-up: disposing of objects and disposing of the Engine");
    math.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform math recognition");
  }
  
  private final static void parseInput(final String path, MathDocument math)
  {
    final Ink ink = (Ink)EngineObject.load(math.getEngine(), path);
    
    for (int i = 0; i < ink.getStrokeCount(); ++i)
    {
      final InkStroke stroke = ink.getStrokeAt(i);
      
      math.addStroke(stroke.getValuesAsFloat(0), stroke.getValuesAsFloat(1));
      
      stroke.dispose();
    }
  
    ink.dispose();
  }
  
} // MathRecognition
