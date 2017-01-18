package text;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.engine.Iterator;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.text.CandidateIterator;
import com.myscript.text.RecognitionResult;
import com.myscript.text.Resource;
import com.myscript.text.SegmentIterator;
import com.myscript.text.StructuredInputRecognizer;
import common.CFG;

/**
 * The <code>Transliteration</code> example code shows you how to
 * perform handwriting recognition and transliteration on digital ink.
 */
public class Transliteration
{
  private final static int MAX_TRANSLITERATION_CANDIDATE_COUNT = 5;
  
  /**
   * Main entry point for the <code>Transliteration</code> example program.
   */
  public static final void main(String[] args) throws IOException
  {
    String str = null;
    
    if (CFG.getLang().equals("zh_CN"))
    {
      if (args.length == 0)
      {
        args = new String[1];
        args[0] = CFG.DATA_DIR + "/transliteration_zh_CN.ink";
      }
      
      str = "nali ma?";
    }
    else if (CFG.getLang().equals("ja_JP"))
    {
      if (args.length == 0)
      {
        args = new String[1];
        args[0] = CFG.DATA_DIR + "/transliteration_ja_JP.ink";
      }
      
      str = "ですね。";
    }
    else
    {
      System.err.println("/!\\ transliteration is only supported in zh_CN or ja_JP");
      System.exit(-1);      
    }
    
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new StructuredInputRecognizer");
    final StructuredInputRecognizer recognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . StructuredInputRecognizer created successfully");
    System.out.println();

    System.out.println("3) enabling transliteration");
    recognizer.setEnableTransliteration(true);
    System.out.println(" . property set successfully");
    System.out.println();
    
    System.out.println("4) loading and attaching resources to the recognizer");
    final Resource akCur = (Resource)EngineObject.load(engine, CFG.getAkCurRes());
    System.out.println(" . CursiveAlphabetKnowledge resource loaded successfully");
    recognizer.attach(akCur);
    System.out.println(" . CursiveAlphabetKnowledge resource attached successfully");
    System.out.println(" . disposing of the CursiveAlphabetKnowledge resource (not needed anymore)");
    akCur.dispose();
    System.out.println(" . CursiveAlphabetKnowledge resource disposed of successfully");
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
    
    System.out.println("5) creating a new FloatStructuredInput");
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println();

    System.out.println("6) setting the input as the source of the recognizer");
    recognizer.setSource(input);
    System.out.println(" . input successfully set as the recognizer's source");
    System.out.println();
    
    System.out.println("7) processing input files");

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

      if (str != null)
        input.addString(str);
      
      System.out.println(" . running the handwriting recognition process");
      recognizer.run();
      System.out.println("   . handwriting recognition performed successfully");
      System.out.println();
      
      System.out.println(" . retrieving the recognition result");
      final RecognitionResult result = recognizer.getResult();
      System.out.println("   . RecognitionResult retrieved successfully");
      System.out.println();

      System.out.println(" . retrieving the first candidate of the RecognitionResult");
      final CandidateIterator textCandidateIterator = result.getCandidates();
      if (!textCandidateIterator.isAtEnd())
        System.out.println("   . input recognized as: \"" + textCandidateIterator.getLabel() + '"');
      else
        System.out.println("   . sorry, unable to recognize the input");
      System.out.println();
      
      System.out.println(" . iterating over word segments and displaying transliteration candidates");
      final SegmentIterator wordSegmentIterator = textCandidateIterator.getSegments();

      System.out.println("   . ");

      while (!wordSegmentIterator.isAtEnd())
      {
        final CandidateIterator selectedWordCandidate = getSelectedCandidate(wordSegmentIterator);
        final String label = selectedWordCandidate.getLabel();

        wordSegmentIterator.next();
        final boolean lastWord = wordSegmentIterator.isAtEnd();

        final boolean isSpaceSegment = isSpace(label);
        if(!isSpaceSegment)
        {
          if (lastWord)
          {
            System.out.print("   └── \"");
            System.out.print(label);
            System.out.println("\"");
          }
          else
          {
            System.out.print("   ├── \"");
            System.out.print(label);
            System.out.println("\"");
          }

          final CandidateIterator transliterationCandidateIterator = selectedWordCandidate.getTransliterationCandidates();
          if (!transliterationCandidateIterator.isAtEnd())
          {
            final int selected = selectedWordCandidate.getSelectedTransliterationCandidateIndex();

            int j = 0;
            while (!transliterationCandidateIterator.isAtEnd())
            {
              boolean lastTransliterationCandidate;

              if (lastWord)
                System.out.print("   └──");
              else
                System.out.print("   │  ");

              final String label_ = transliterationCandidateIterator.getLabel();
              ++j;
              transliterationCandidateIterator.next();
              lastTransliterationCandidate = (j == MAX_TRANSLITERATION_CANDIDATE_COUNT) || transliterationCandidateIterator.isAtEnd();

              if (lastTransliterationCandidate)
              {
                System.out.print(" └── \"");
                System.out.print(label_);
                System.out.print("\"");
              }
              else
              {
                System.out.print(" ├── \"");
                System.out.print(label_);
                System.out.print("\"");
              }

              if (selected == j -1)
                System.out.print(" (selected)");
              System.out.println();

              if (j == MAX_TRANSLITERATION_CANDIDATE_COUNT && j >= selected)
                break;
            }

            transliterationCandidateIterator.dispose();
          }
        }

        if (!lastWord && !isSpaceSegment)
          System.out.println("   │");

        selectedWordCandidate.dispose();
      }
      System.out.println();

      wordSegmentIterator.dispose();
      textCandidateIterator.dispose();
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
    System.out.println(" you just learnt how to perform handwriting recognition and transliteration on digital ink");
  }

  /**
   * Moves the given iterator by <code>count</code> steps.
   * 
   * @param iterator an <code>Iterator</code> object.
   * @param count the number of steps to perform.
   * 
   * @return The number of steps actually performed.
   */
  private static final int moveIterator(final Iterator iterator,
                                        final int count)
  {
    assert (iterator != null) : "invalid iterator: null is not allowed";
    assert (count >= 0) : "invalid count: must be a positive value";
    
    int i;
    
    for (i = 0; i < count && !iterator.isAtEnd(); ++i)
      iterator.next();
    
    return i;
  }
  
  /**
   * Returns a candidate iterator positioned on the selected candidate of the
   * specified segment.
   *
   * @param segment a <code>SegmentIterator</code> object.
   *
   * @return a <code>CandidateIterator</code> object positioned on the selected
   * candidate of the specified segment.
   */
  private static final CandidateIterator getSelectedCandidate(final SegmentIterator segment)
  {
    assert (segment != null) : "invalid segment: null is not allowed";

    final CandidateIterator candidate = segment.getCandidates();
    moveIterator(candidate, segment.getSelectedCandidateIndex());
    
    return candidate;
  }
  
  /**
   * Tells if the specified label is a space.
   *
   * @return <code>true</code> if the specified label is a space, otherwise
   * <code>false</code>.
   */
  private static final boolean isSpace(final String label)
  {
    assert (label != null) : "invalid label: null is not allowed";

    return label.equals("") || label.equals(" ") || label.equals("\u00A0");
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
  
} // Transliteration
