package shape;

import java.text.NumberFormat;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.shape.ShapeBeautifier;
import com.myscript.shape.ShapeCandidate;
import com.myscript.shape.ShapeDocument;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeModel;
import com.myscript.shape.ShapeRecognized;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.shape.ShapeSegment;
import common.CFG;

/**
 * The <code>IncrementalShapeRecognitionBeautification</code> example code shows
 * you how to perform incremental shape recognition and beautification.
 */
public class IncrementalShapeRecognitionBeautification
{
  /**
   * Main entry point for the
   * <code>IncrementalShapeRecognitionBeautification</code> example program.
   *
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args)
  {
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new ShapeRecognizer and a new ShapeBeautifier");
    final ShapeRecognizer recognizer = ShapeRecognizer.create(engine);
    System.out.println(" . ShapeRecognizer created successfully");
    final ShapeBeautifier beautifier = ShapeBeautifier.create(engine);
    System.out.println(" . ShapeBeautifier created successfully");
    System.out.println();

    System.out.println("3) loading and attaching the ShapeKnowledge to the ShapeRecognizer");
    final ShapeKnowledge shapeKnowledge = (ShapeKnowledge)EngineObject.load(engine, CFG.getShapeKnowledgeRes());
    System.out.println(" . ShapeKnowledge resource loaded successfully");
    recognizer.attach(shapeKnowledge);
    System.out.println(" . ShapeKnowledge resource attached successfully to the ShapeRecognizer");
    beautifier.attach(shapeKnowledge);
    System.out.println(" . ShapeKnowledge resource attached successfully to the ShapeBeautifier");
    System.out.println(" . disposing of the ShapeKnowledge resource (not needed anymore)");
    shapeKnowledge.dispose();
    System.out.println(" . ShapeKnowledge resource disposed of successfully");
    System.out.println();
    
    System.out.println("4) creating a new ShapeDocument");
    final ShapeDocument document = ShapeDocument.create(engine);
    System.out.println(" . ShapeDocument created successfully");
    System.out.println();

    System.out.println("5) fueling the ShapeDocument with digital ink");
    System.out.println(" . adding a rectangle");
    System.out.println("   ,--------.");
    System.out.println("   |        |");
    System.out.println("   |        |");
    System.out.println("   '--------'");
    for (int i = 0; i < Ink.RECTANGLE.length; ++i)
    {
      float[] xy = Ink.RECTANGLE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    
    System.out.println(" . adding an ALMOST horizontal line");
    System.out.println("   ,--------.    ,----");
    System.out.println("   |        | ---");
    System.out.println("   |        |");
    System.out.println("   '--------'");
    for (int i = 0; i < Ink.LINE.length; ++i)
    {
      float[] xy = Ink.LINE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    
    System.out.println(" . adding a circle");
    System.out.println("                         ,---.");
    System.out.println("   ,--------.    ,----  /     \\");
    System.out.println("   |        | ---      |       |");
    System.out.println("   |        |           \\     /");
    System.out.println("   '--------'            '---'");
    for (int i = 0; i < Ink.CIRCLE.length; ++i)
    {
      float[] xy = Ink.CIRCLE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();
    
    System.out.println("6) running the shape recognition and beautification process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    beautifier.process(document);
    System.out.println(" . shape beautification performed successfully");
    System.out.println();
    
    System.out.println("7) retrieving the recognition and beautification result");
    printResult(document);
    System.out.println();

    System.out.println("8) fueling the ShapeDocument with more digital ink");
    System.out.println(" . adding an arrow head");
    System.out.println(" . please note that in the typical incremental scenario, the user");
    System.out.println("   draws the arrow head on top of a render of the previously recognized");
    System.out.println("   and beautified diagram, the ink is not displayed anymore");
    System.out.println(" . as you'll see next, the ink for the arrow head is in fact drawn");
    System.out.println("   below the right extremity of that almost horizontal line");
    System.out.println("                         ,---.");
    System.out.println("   ,--------.           /     \\");
    System.out.println("   |        | -------->|       |");
    System.out.println("   |        |           \\     /");
    System.out.println("   '--------'            '---'");
    for (int i = 0; i < Ink.ARROW_HEAD.length; ++i)
    {
      float[] xy = Ink.ARROW_HEAD[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();
    
    System.out.println("9) running the shape recognition and beautification process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    beautifier.process(document);
    System.out.println(" . shape beautification performed successfully");
    System.out.println();
    
    System.out.println("10) retrieving the recognition and beautification result");
    printResult(document);
    System.out.println();

    System.out.println("11) clearing the ShapeDocument");
    document.clear();
    System.out.println(" . ShapeDocument cleared successfully");
    System.out.println();
    
    System.out.println("12) fueling the ShapeDocument with digital ink AS A WHOLE");
    System.out.println(" . adding a rectangle");
    System.out.println("   ,--------.");
    System.out.println("   |        |");
    System.out.println("   |        |");
    System.out.println("   '--------'");
    for (int i = 0; i < Ink.RECTANGLE.length; ++i)
    {
      float[] xy = Ink.RECTANGLE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    
    System.out.println(" . adding an ALMOST horizontal line");
    System.out.println("   ,--------.    ,----");
    System.out.println("   |        | ---");
    System.out.println("   |        |");
    System.out.println("   '--------'");
    for (int i = 0; i < Ink.LINE.length; ++i)
    {
      float[] xy = Ink.LINE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    
    System.out.println(" . adding a circle");
    System.out.println("                         ,---.");
    System.out.println("   ,--------.    ,----  /     \\");
    System.out.println("   |        | ---      |       |");
    System.out.println("   |        |           \\     /");
    System.out.println("   '--------'            '---'");
    for (int i = 0; i < Ink.CIRCLE.length; ++i)
    {
      float[] xy = Ink.CIRCLE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    
    System.out.println(" . adding an arrow head (as if the line was horizontal)");
    System.out.println("                         ,---.");
    System.out.println("   ,--------.    ,----  /     \\");
    System.out.println("   |        | ---     >|       |");
    System.out.println("   |        |           \\     /");
    System.out.println("   '--------'            '---'");
    for (int i = 0; i < Ink.ARROW_HEAD.length; ++i)
    {
      float[] xy = Ink.ARROW_HEAD[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();
    
    System.out.println("13) running the shape recognition and beautification process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    beautifier.process(document);
    System.out.println(" . shape beautification performed successfully");
    System.out.println();
    
    System.out.println("14) retrieving the recognition and beautification result");
    printResult(document);
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("15) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform incremental recognition and beautification");
  }
  
  private static void printResult(final ShapeDocument document)
  {
    System.out.print(" . retrieving the number of ShapeSegments in the ShapeDocument: ");
    final int segmentCount = document.getSegmentCount();
    System.out.println(segmentCount);
    
    final NumberFormat nf = NumberFormat.getInstance();
    nf.setGroupingUsed(false);
    
    System.out.println(" . iterating over the ShapeSegments:");
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

        candidate.dispose();
      }

      segment.dispose();
    }
  }
  
} // MultipleShapesRecognitionBeautification
