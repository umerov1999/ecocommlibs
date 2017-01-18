package prediction;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.engine.IFloatStroke;
import com.myscript.prediction.PredictionCandidateFlags;
import com.myscript.prediction.Predictor;
import com.myscript.text.CandidateFlags;
import com.myscript.text.CandidateIterator;
import com.myscript.text.RecognitionResult;
import com.myscript.text.Resource;
import com.myscript.text.SegmentIterator;
import com.myscript.text.StructuredInputRecognizer;
import common.CFG;
import common.Ink;

/**
 * The <code>ConjointRecognitionCompletionAndPrediction</code> example code
 * shows you how to perform conjoint handwriting recognition, word completion
 * and prediction.
 */
public class ConjointRecognitionCompletionAndPrediction
{
  /**
   * Main entry point for the
   * <code>ConjointRecognitionCompletionAndPrediction</code> example program.
   *
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args) throws IOException
  {
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new StructuredInputRecognizer");
    final StructuredInputRecognizer recognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . StructuredInputRecognizer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resources to the recognizer");
    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkRes());
    System.out.println(" . AlphabetKnowledge resource loaded successfully");
    recognizer.attach(ak);
    System.out.println(" . AlphabetKnowledge resource attached successfully");
    System.out.println(" . disposing of the AlphabetKnowledge resource (not needed anymore)");
    ak.dispose();
    System.out.println(" . AlphabetKnowledge resource disposed of successfully");
    
    final Resource lk = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . LinguisticKnowledge resource loaded successfully");
    recognizer.attach(lk);
    System.out.println(" . LinguisticKnowledge resource attached successfully");
    System.out.println(" . disposing of the LinguisticKnowledge resource (not needed anymore)");
    lk.dispose();
    System.out.println(" . LinguisticKnowledge resource disposed of successfully");
    System.out.println();
    
    System.out.println("4) creating a new FloatStructuredInput");
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println();

    System.out.println("5) fueling the input with digital ink");
    final File documentInkFile = new File(CFG.DATA_DIR + "/good_aftern.ink");
    Ink.parse(documentInkFile, new Ink.IParseHandler()
    {
      public void addStroke(IFloatStroke stroke)
      {
        input.addStroke(stroke);
      }
    });
    System.out.println(" .input successfully fueled");
    System.out.println();    
    
    System.out.println("6) setting the input as the source of the recognizer");
    recognizer.setSource(input);
    System.out.println(" . input successfully set as the recognizer's source");
    System.out.println();

    System.out.println("7) creating a new Predictor");
    final Predictor predictor = Predictor.create(engine);
    System.out.println(" . Predictor created successfully");
    System.out.println();

    System.out.println("8) configuring the Predictor");
    predictor.setWordCompletionCandidateListSize(2);
    predictor.setWordPredictionCandidateListSize(2);
    System.out.println(" . Predictor successfully configured");
    System.out.println();

    System.out.println("9) attaching the predictor to the recognizer");
    recognizer.attach(predictor);
    System.out.println(" . Predictor object attached successfully");
    System.out.println();
    
    System.out.println("10) running the handwriting recognition process");
    recognizer.run();
    System.out.println(" . handwriting recognition performed successfully");
    System.out.println();
    
    System.out.println("11) retrieving the RecognitionResult");
    RecognitionResult result = recognizer.getResult();
    System.out.println(" . RecognitionResult retrieved successfully");
    System.out.println();
    
    System.out.println("12) retrieving the first candidate of the RecognitionResult");
    final CandidateIterator textCandidateIterator = result.getCandidates();
    if (textCandidateIterator.isAtEnd())
      System.out.println(" . sorry, unable to recognize the input");
    else
    {
      SegmentIterator wordIterator;
      int wordCount, padding;

      wordIterator = textCandidateIterator.getSegments();

      wordCount = 0;
      while (!wordIterator.isAtEnd())
      {
        ++wordCount;
        wordIterator.next();
      }
      wordIterator.dispose();


      System.out.print(" . ");
      padding = 3;

      wordIterator = textCandidateIterator.getSegments();
      // only the last word segment has completions / predictions candidates
      for (int i = 0; i < wordCount - 1; ++i) 
      {
        final CandidateIterator wordCandidateIterator = wordIterator.getCandidates();

        int selected = wordIterator.getSelectedCandidateIndex();
        while (selected-- != 0)
          wordCandidateIterator.next();

        // print one character at a time
        // and keep track of number of characters printed
        final SegmentIterator characterIterator = wordCandidateIterator.getSegments();
        while (!characterIterator.isAtEnd())
        {
          final CandidateIterator characterCandidateIterator = characterIterator.getCandidates();

          selected = characterIterator.getSelectedCandidateIndex();
          while (selected-- != 0)
            characterCandidateIterator.next();

          System.out.print(characterCandidateIterator.getLabel());
          ++padding;
          
          characterCandidateIterator.dispose();
          characterIterator.next();
        }
        characterIterator.dispose();

        wordCandidateIterator.dispose();
        wordIterator.next();
      }

      // last word segment 
      {
        final CandidateIterator wordCandidateIterator = wordIterator.getCandidates();
        
        while (!wordCandidateIterator.isAtEnd())
        {
          System.out.println(wordCandidateIterator.getLabel());

          for (int i = 0; i < padding; ++i)
            System.out.print(' ');

          final SegmentIterator characterIterator = wordCandidateIterator.getSegments();
          while (!characterIterator.isAtEnd())
          {
            final CandidateIterator characterCandidateIterator = characterIterator.getCandidates();

            int selected = characterIterator.getSelectedCandidateIndex();
            while (selected-- != 0)
              characterCandidateIterator.next();

            CandidateFlags flags = characterCandidateIterator.getFlags();
            if (flags.has(CandidateFlags.OMITTED))
            {
              if (flags.has(PredictionCandidateFlags.COMPLETED))
                System.out.print('-');
              else if (flags.has(PredictionCandidateFlags.PREDICTED))
                System.out.print('+');
            }
            else
              System.out.print(' ');

            characterCandidateIterator.dispose();
            characterIterator.next();
          }
          characterIterator.dispose();

          System.out.println();
          for (int i = 0; i < padding; ++i)
            System.out.print(' ');

          wordCandidateIterator.next();
        }
        wordCandidateIterator.dispose();
      }
      wordIterator.dispose();
    }
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("13) cleaning-up: disposing of objects and disposing of the Engine");
    textCandidateIterator.dispose();
    result.dispose();
    predictor.dispose();
    input.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform conjoing handwriting recognition, word completion and prediction");
  }
  
} // ConjointRecognitionCompletionAndPrediction
