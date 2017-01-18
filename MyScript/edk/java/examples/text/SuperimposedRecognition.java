package text;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.text.CandidateIterator;
import com.myscript.text.RecognitionResult;
import com.myscript.text.Resource;
import com.myscript.text.StructuredInputRecognizer;
import common.CFG;

/**
 * The <code>SuperimposedRecognition</code> example code shows you how to
 * perform handwriting recognition on superimposed digital ink.
 */
public class SuperimposedRecognition
{
  /**
   * Main entry point for the <code>SuperimposedRecognition</code> example
   * program.
   */
  public static final void main(String[] args) throws IOException
  {
    if (args.length == 0)
    {
      args = new String[1];
      args[0] = CFG.DATA_DIR + "/hello_world_superimposed.ink";
    }
    
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new StructuredInputRecognizer");
    final StructuredInputRecognizer recognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . StructuredInputRecognizer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resources to the recognizer");
    final Resource akCur = (Resource)EngineObject.load(engine, CFG.getAkSuperimposedRes());
    System.out.println(" . SuperimposedAlphabetKnowledge resource loaded successfully");
    recognizer.attach(akCur);
    System.out.println(" . SuperimposedAlphabetKnowledge resource attached successfully");
    System.out.println(" . disposing of the SuperimposedAlphabetKnowledge resource (not needed anymore)");
    akCur.dispose();
    System.out.println(" . SuperimposedAlphabetKnowledge resource disposed of successfully");
    final Resource lkText = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . Main LinguisticKnowledge resource loaded successfully");
    recognizer.attach(lkText);
    System.out.println(" . Main LinguisticKnowledge resource attached successfully");
    System.out.println(" . disposing of the main LinguisticKnowledge resource (not needed anymore)");
    lkText.dispose();
    System.out.println(" . Main LinguisticKnowledge resource disposed of successfully");
    if (!CFG.getLang().contains("en"))
    {
      final String path;

      if (CFG.getLang().contains("zh") || CFG.getLang().contains("ja") || CFG.getLang().contains("ko"))
        path = CFG.getLkEnglishSecondary2Res();
      else if (CFG.getLang().contains("hi"))
        path = CFG.getLkEnglishSecondary3Res();
      else
        path = CFG.getLkEnglishSecondary1Res();

      final Resource lkEnglishSecondary = (Resource)EngineObject.load(engine, path);
      System.out.println(" . Secondary English LinguisticKnowledge resource loaded successfully");
      recognizer.attach(lkEnglishSecondary);
      System.out.println(" . Secondary English LinguisticKnowledge resource attached successfully");
      System.out.println(" . disposing of the secondary LinguisticKnowledge resource (not needed anymore)");
      lkEnglishSecondary.dispose();
      System.out.println(" . Secondary LinguisticKnowledge resource disposed of successfully");
    }
    System.out.println();
    
    System.out.println("4) creating a new FloatStructuredInput");
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println();

    System.out.println("5) setting the input as the source of the recognizer");
    recognizer.setSource(input);
    System.out.println(" . input successfully set as the recognizer's source");
    System.out.println();
    
    System.out.println("6) processing input files");

    for (int i = 0; i < args.length; ++i)
    {
      if (!new File(args[i]).exists())
      {
        System.err.println("missing ink file: " + args[i]);
        System.exit(-1);
      }
      
      System.out.println(" . parsing: " + new File(args[i]).getCanonicalPath());
      parseInput(args[i], input);
      System.out.println("   . input parsed successfully");
      System.out.println();

      System.out.println(" . running the handwriting recognition process");
      recognizer.run();
      System.out.println("   . handwriting recognition performed successfully");
      System.out.println();
      
      System.out.println(" . retrieving the recognition result");
      final RecognitionResult result = recognizer.getResult();
      System.out.println("   . RecognitionResult retrieved successfully");
      System.out.println();

      System.out.println(" . retrieving the first candidate of the RecognitionResult");
      final CandidateIterator iterator = result.getCandidates();
      if (!iterator.isAtEnd())
        System.out.println("   . input recognized as: \"" + iterator.getLabel() + '"');
      else
        System.out.println("   . sorry, unable to recognize the input");
      System.out.println();
      
      iterator.dispose();
      result.dispose();
      
      input.clear(false);
    }
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("7) cleaning-up: disposing of objects and disposing of the Engine");
    input.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform handwriting recognition on superimposed digital ink");
  }

  private final static void parseInput(final String path, FloatStructuredInput input)
  {
    final Ink ink = (Ink)EngineObject.load(input.getEngine(), path);
    
    for (int i = 0; i < ink.getStrokeCount(); ++i)
    {
      final InkStroke stroke = ink.getStrokeAt(i);
      
      input.addStroke(stroke.getValuesAsFloat(0), stroke.getValuesAsFloat(1));
      
      stroke.dispose();
    }
  
    ink.dispose();
  }
  
} // SuperimposedRecognition
