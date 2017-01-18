package shape;

import java.text.NumberFormat;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.shape.DecorationType;
import com.myscript.shape.ShapeBeautifier;
import com.myscript.shape.ShapeCandidate;
import com.myscript.shape.ShapeDecoratedLine;
import com.myscript.shape.ShapeDecoratedLineData;
import com.myscript.shape.ShapeDocument;
import com.myscript.shape.ShapeEllipticArc;
import com.myscript.shape.ShapeEllipticArcData;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeLine;
import com.myscript.shape.ShapeLineData;
import com.myscript.shape.ShapeModel;
import com.myscript.shape.ShapePointData;
import com.myscript.shape.ShapePrimitive;
import com.myscript.shape.ShapeRecognized;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.shape.ShapeSegment;
import common.CFG;

/**
 * The <code>MultipleShapesRecognitionBeautification</code> example code shows
 * you how to perform recognition and beautification of multiple handwritten
 * shapes.
 */
public class MultipleShapesRecognitionBeautification
{
  /**
   * Main entry point for the
   * <code>MultipleShapesRecognitionBeautification</code> example program.
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
    
    System.out.println(" . adding an arrow");
    System.out.println("   ,--------.");
    System.out.println("   |        |");
    System.out.println("   |        | ------>");
    System.out.println("   '--------'");
    for (int i = 0; i < Ink.ARROW.length; ++i)
    {
      float[] xy = Ink.ARROW[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    
    System.out.println(" . adding a circle");
    System.out.println("                        ,---.");
    System.out.println("   ,--------.          /     \\");
    System.out.println("   |        |         |       |");
    System.out.println("   |        | ------>  \\     /");
    System.out.println("   '--------'           '---'");
    for (int i = 0; i < Ink.CIRCLE.length; ++i)
    {
      float[] xy = Ink.CIRCLE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    
    System.out.println(" . adding a triangle");
    System.out.println("                        ,---.");
    System.out.println("   ,--------.          /     \\");
    System.out.println("   |        |         |       |");
    System.out.println("   |        | ------>  \\     /");
    System.out.println("   '--------'           '---'");
    System.out.println("                  .");
    System.out.println("                 / \\");
    System.out.println("                /   \\");
    System.out.println("               '-----'");
    for (int i = 0; i < Ink.TRIANGLE.length; ++i)
    {
      float[] xy = Ink.TRIANGLE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();
    
    System.out.println("6) running the shape recognition process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    System.out.println();
    
    System.out.println("7) retrieving the recognition result");
    printResult(document);
    System.out.println();

    System.out.println("8) running the shape beautification process");
    beautifier.process(document);
    System.out.println(" . shape beautification performed successfully");
    System.out.println();
    
    System.out.println("9) retrieving the beautification result");
    printResult(document);
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("10) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform multiple shapes recognition and beautification");
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
          
          final int primitiveCount = ((ShapeRecognized)candidate).getPrimitiveCount();
          
          nf.setMinimumFractionDigits(2);
          nf.setMaximumFractionDigits(2);
          for (int j = 0; j < primitiveCount; ++j)
          {
            final ShapePrimitive primitive = ((ShapeRecognized)candidate).getPrimitiveAt(j);
           
            if (primitive instanceof ShapeLine)
            {
              final ShapeLineData data = ((ShapeLine)primitive).getData();
              final ShapePointData p1 = data.getP1();
              final ShapePointData p2 = data.getP2();
              
              System.out.println( "     . line: (" + nf.format(p1.getX()) + ", " + nf.format(p1.getY()) +
                                  "), (" + nf.format(p2.getX()) + ", " + nf.format(p2.getY()) + ')');
            }
            else if (primitive instanceof ShapeEllipticArc)
            {
              final ShapeEllipticArcData data = ((ShapeEllipticArc)primitive).getData();
              final ShapePointData center = data.getCenter();
             
              System.out.println( "     . arc: (" + nf.format(center.getX()) + ", " + nf.format(center.getY()) + ')' +
                                  ", " + nf.format(data.getMinRadius()) + 
                                  ", " + nf.format(data.getMaxRadius()) +
                                  ", " + nf.format(data.getOrientation()) +
                                  ", " + nf.format(data.getStartAngle()) + 
                                  ", " + nf.format(data.getSweepAngle()));
            }
            else if (primitive instanceof ShapeDecoratedLine)
            {
              final ShapeDecoratedLineData data = ((ShapeDecoratedLine)primitive).getData();
              final ShapePointData p1 = data.getLine().getP1();
              final ShapePointData p2 = data.getLine().getP2();
              
              System.out.println( "     . decorated line: (" + nf.format(p1.getX()) + ", " + nf.format(p1.getY()) + ')' + (data.getP1Decoration().equals(DecorationType.ARROW_HEAD) ? " head" : "") +
                                  ", (" + nf.format(p2.getX()) + ", " + nf.format(p2.getY()) + ')' + (data.getP2Decoration().equals(DecorationType.ARROW_HEAD) ? " head" : ""));
              
            }
            
            primitive.dispose();
          }
          
          model.dispose();
        }

        candidate.dispose();
      }

      segment.dispose();
    }
  }

} // MultipleShapesRecognitionBeautification
