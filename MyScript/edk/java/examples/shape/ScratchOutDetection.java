package shape;

import java.text.NumberFormat;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.shape.ShapeCandidate;
import com.myscript.shape.ShapeDocument;
import com.myscript.shape.ShapeErased;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeModel;
import com.myscript.shape.ShapeRecognized;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.shape.ShapeScratchOut;
import com.myscript.shape.ShapeSegment;
import common.CFG;

/**
 * The <code>ScratchOutDetection</code> example code shows you how to handle
 * scratch out.
 */
public class ScratchOutDetection
{
  /**
   * Main entry point for the <code>ScratchOutDetection</code> example program.
   *
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args)
  {
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new ShapeRecognizer");
    final ShapeRecognizer recognizer = ShapeRecognizer.create(engine);
    System.out.println(" . ShapeRecognizer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching the ShapeKnowledge to the ShapeRecognizer");
    final ShapeKnowledge shapeKnowledge = (ShapeKnowledge)EngineObject.load(engine, CFG.getShapeKnowledgeRes());
    System.out.println(" . ShapeKnowledge resource loaded successfully");
    recognizer.attach(shapeKnowledge);
    System.out.println(" . ShapeKnowledge resource attached successfully");
    System.out.println(" . disposing of the ShapeKnowledge resource (not needed anymore)");
    shapeKnowledge.dispose();
    System.out.println(" . ShapeKnowledge resource disposed of successfully");
    System.out.println();
    
    System.out.println("4) creating a new ShapeDocument");
    final ShapeDocument document = ShapeDocument.create(engine);
    System.out.println(" . ShapeDocument created successfully");
    System.out.println();

    System.out.println("5) fueling the ShapeDocument with digital ink");
    System.out.println(" . adding ink that ALMOST forms a square");
    System.out.println("   ,-------------");
    System.out.println("   |      |");
    System.out.println("   |      |");
    System.out.println("   '------'");
    document.addStroke(Ink.SCRATCH_OUT[0], 0, 2, Ink.SCRATCH_OUT[0], 1, 2, Ink.SCRATCH_OUT[0].length / 2);
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();
    
    System.out.println("6) running the shape recognition process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    System.out.println();
    
    System.out.println("7) retrieving the recognition result");
    printResult(document);
    System.out.println();

    System.out.println("8) fueling the ShapeDocument with more digital ink");
    System.out.println(" . scratching out the previous ink");
    System.out.println("   ,------/-/-/-/");
    System.out.println("   |      |");
    System.out.println("   |      |");
    System.out.println("   '------'");
    document.addStroke(Ink.SCRATCH_OUT[1], 0, 2, Ink.SCRATCH_OUT[1], 1, 2, Ink.SCRATCH_OUT[1].length / 2);
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();

    System.out.println("9) running the shape recognition process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    System.out.println();
    
    System.out.println("10) retrieving the recognition result");
    printResult(document);
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("11) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to handle scratch out");
  }
  
  private static void printResult(final ShapeDocument document)
  {
    System.out.print(" . retrieving the number of ShapeSegments in the ShapeDocument: ");
    final int segmentCount = document.getSegmentCount();
    System.out.println(segmentCount);
    
    System.out.println(" . iterating over the ShapeSegments:");
    
    final NumberFormat nf = NumberFormat.getInstance();
    nf.setGroupingUsed(false);
    
    for (int i = 0; i < segmentCount; ++i)
    {
      final ShapeSegment segment = document.getSegmentAt(i);
      final int candidateCount = segment.getCandidateCount();

      if (candidateCount != 0)
      {
        final ShapeCandidate candidate = segment.getCandidateAt(0);

        if (candidate instanceof ShapeRecognized)
        {
          final ShapeModel model = ((ShapeRecognized)candidate).getModel();

          final float rs = ((ShapeRecognized)candidate).getResemblanceScore();
          final float nrs = ((ShapeRecognized)candidate).getNormalizedRecognitionScore();

          nf.setMinimumFractionDigits(4);
          nf.setMaximumFractionDigits(4);
          System.out.println("   . segment " + i + ", best candidate: " + model.getLabel() +" (rs:" + nf.format(rs) + " nrs:" + nf.format(nrs) + ')');
          
          model.dispose();
        }
        else if (candidate instanceof ShapeScratchOut)
        {
          System.out.println("   . segment " + i + ", scratch out");
        }
        else if (candidate instanceof ShapeErased)
        {
          System.out.println("   . segment " + i + ", erased");
        }

        candidate.dispose();
      }

      segment.dispose();
    }
  }
  
} // ScratchOutDetection
