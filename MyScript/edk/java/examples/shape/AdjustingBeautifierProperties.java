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
 * The <code>AdjustingBeautifierProperties</code> example code shows you how to
 * adjust beautifier properties.
 */
public class AdjustingBeautifierProperties
{
  /**
   * Main entry point for the <code>AdjustingBeautifierProperties</code> example
   * program.
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
    ShapeDocument document = ShapeDocument.create(engine);
    System.out.println(" . ShapeDocument created successfully");
    System.out.println();

    System.out.println("5) fueling the ShapeDocument with digital ink");
    System.out.println(" . we are going to adjust the alignment with the x and y axis");
    System.out.println(" . adding a rectangle (which is in fact not aligned with the x and y axis");
    System.out.println("   ,--------.");
    System.out.println("   |        |");
    System.out.println("   |        |");
    System.out.println("   '--------'");
    for (int i = 0; i < Ink.RECTANGLE.length; ++i)
    {
      float[] xy = Ink.RECTANGLE[i];

      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();

    System.out.println("6) running the shape recognition and beautification process");
    recognizer.process(document);
    beautifier.process(document);
    System.out.println(" . shape recognition and beautification performed successfully");
    System.out.println();

    System.out.println("7) retrieving the recognition and beautification result");
    printResult(document, 0);
    System.out.println();

    System.out.println("8) un-freezing the beautification");
    unfreezeBeautification(document);
    System.out.println();

    System.out.println("9) adjusting ShapeBeautifier's alignment strength to 0 (disabled)");
    beautifier.setAlignmentStrength(0);
    System.out.println(" . alignment strength adjusted successfully");
    System.out.println();

    System.out.println("10) running the beautification process again");
    beautifier.process(document);
    System.out.println(" . shape beautification performed successfully");
    System.out.println();

    System.out.println("11) retrieving the recognition and beautification result");
    printResult(document, 0);
    System.out.println();

    System.out.println("12) clearing the ShapeDocument");
    document.clear();
    System.out.println(" . ShapeDocument cleared successfully");
    System.out.println();

    System.out.println("13) fueling the ShapeDocument with digital ink");
    System.out.println(" . we are going to adjust the connection between shapes");
    System.out.println(" . adding an arrow");
    System.out.println("   ------>");
    for (int i = 0; i < Ink.ARROW.length; ++i)
    {
      float[] xy = Ink.ARROW[i];

      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }

    System.out.println(" . adding a circle");
    System.out.println("             ,---.");
    System.out.println("            /     \\");
    System.out.println("           |       |");
    System.out.println("   ------>  \\     /");
    System.out.println("             '---'");
    for (int i = 0; i < Ink.CIRCLE.length; ++i)
    {
      float[] xy = Ink.CIRCLE[i];

      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();

    System.out.println("14) resetting the ShapeBeautifier's alignment strength to its default value");
    beautifier.resetAlignmentStrength();
    System.out.println(" . alignment strength resetted successfully");
    System.out.println();

    System.out.println("15) running the shape recognition and beautification process");
    recognizer.process(document);
    beautifier.process(document);
    System.out.println(" . shape recognition and beautification performed successfully");
    System.out.println();

    System.out.println("16) retrieving the recognition and beautification result");
    printResult(document, 1);
    System.out.println();

    System.out.println("17) un-freezing the beautification");
    unfreezeBeautification(document);
    System.out.println();

    System.out.println("18) adjusting ShapeBeautifier's connection strength to 0 (disabled)");
    beautifier.setConnectionStrength(0);
    System.out.println(" . connection strength adjusted successfully");
    System.out.println();

    System.out.println("19) running the beautification process again");
    beautifier.process(document);
    System.out.println(" . shape beautification performed successfully");
    System.out.println();

    System.out.println("20) retrieving the recognition and beautification result");
    printResult(document, 1);
    System.out.println();

    System.out.println("21) clearing the ShapeDocument");
    document.clear();
    System.out.println(" . ShapeDocument cleared successfully");
    System.out.println();

    System.out.println("22) fueling the ShapeDocument with digital ink");
    System.out.println(" . we are going to adjust the congruence between shapes");
    System.out.println(" . adding a circle");
    System.out.println("     ,---.");
    System.out.println("    /     \\");
    System.out.println("   |       |");
    System.out.println("    \\     /");
    System.out.println("     '---'");
    document.addStroke(Ink.CIRCLES[0], 0, 2, Ink.CIRCLES[0], 1, 2, Ink.CIRCLES[0].length / 2);

    System.out.println(" . adding another circle");
    System.out.println("               ,---.");
    System.out.println("              /     \\");
    System.out.println("    ,---.    |       |");
    System.out.println("   /     \\    \\     /");
    System.out.println("  |       |    '---'");
    System.out.println("   \\     /");
    System.out.println("    '---'");
    document.addStroke(Ink.CIRCLES[1], 0, 2, Ink.CIRCLES[1], 1, 2, Ink.CIRCLES[1].length / 2);
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();

    System.out.println("23) resetting the ShapeBeautifier's connection strength to its default value");
    beautifier.resetConnectionStrength();
    System.out.println(" . connection strength resetted successfully");
    System.out.println();

    System.out.println("24) running the shape recognition and beautification process");
    recognizer.process(document);
    beautifier.process(document);
    System.out.println(" . shape recognition and beautification performed successfully");
    System.out.println();

    System.out.println("25) retrieving the recognition and beautification result");
    printResult(document, 2);
    System.out.println();

    System.out.println("26) un-freezing the beautification");
    unfreezeBeautification(document);
    System.out.println();

    System.out.println("27) adjusting ShapeBeautifier's congruence strength to 1 (disabled)");
    beautifier.setCongruenceStrength(1);
    System.out.println(" . congruence strength adjusted successfully");
    System.out.println();

    System.out.println("28) running the beautification process again");
    beautifier.process(document);
    System.out.println(" . shape beautification performed successfully");
    System.out.println();

    System.out.println("29) retrieving the recognition and beautification result");
    printResult(document, 2);
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("30) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to adjust ShapeBeautifier properties");
  }

  private static void unfreezeBeautification(final ShapeDocument document)
  {
    final int count = document.getSegmentCount();

    for (int i = 0; i < count; ++i)
    {
      final ShapeSegment segment = document.getSegmentAt(i);

      segment.setFreezeBeautification(false);

      segment.dispose();
    }

    System.out.println(" . beautification un-frozen successfully");
  }

  private static float distance(final ShapePointData p1, final ShapePointData p2)
  {
    return (float)Math.sqrt((p1.getX() - p2.getX())*(p1.getX() - p2.getX()) + (p1.getY() - p2.getY())*(p1.getY() - p2.getY()));
  }

  private static void printResult(final ShapeDocument document, int magic)
  {
    ShapePointData p1 = null;
    ShapePointData p2 = null;
    float radius = 0.0f;

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

          final int primitiveCount = ((ShapeRecognized)candidate).getPrimitiveCount();

          nf.setMinimumFractionDigits(2);
          nf.setMaximumFractionDigits(2);
          for (int j = 0; j < primitiveCount; ++j)
          {
            final ShapePrimitive primitive = ((ShapeRecognized)candidate).getPrimitiveAt(j);

            if (primitive instanceof ShapeLine)
            {
              final ShapeLineData data = ((ShapeLine)primitive).getData();
              final ShapePointData p1_ = data.getP1();
              final ShapePointData p2_ = data.getP2();

              System.out.println( "     . line: (" + nf.format(p1_.getX()) + ", " + nf.format(p1_.getY()) +
                                  "), (" + nf.format(p2_.getX()) + ", " + nf.format(p2_.getY()) + ')');

              if (magic == 0 && j == 0) // alignment
              {
                p1 = p1_;
                p2 = p2_;
              }
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

              if (magic == 1 && model.getLabel().equals("circle")) // connection
              {
                p2 = center;
                radius = data.getMinRadius();
              }
              else if (magic == 2 && model.getLabel().equals("circle")) // congruence
              {
                if (i == 0)
                  p1 = center;
                else
                  p2 = center;
              }
            }
            else if (primitive instanceof ShapeDecoratedLine)
            {
              final ShapeDecoratedLineData data = ((ShapeDecoratedLine)primitive).getData();
              final ShapePointData p1_ = data.getLine().getP1();
              final ShapePointData p2_ = data.getLine().getP2();

              System.out.println( "     . decorated line: (" + nf.format(p1_.getX()) + ", " + nf.format(p1_.getY()) + ')' + (data.getP1Decoration().equals(DecorationType.ARROW_HEAD) ? " head" : "") +
                                  ", (" + nf.format(p2_.getX()) + ", " + nf.format(p2_.getY()) + ')' + (data.getP2Decoration().equals(DecorationType.ARROW_HEAD) ? " head" : ""));

              if (magic == 1 && model.getLabel().equals("arrow")) // connection
              {
                p1 = p2_;
              }
            }

            primitive.dispose();
          }

          model.dispose();
        }

        candidate.dispose();
      }

      segment.dispose();
    }

    if (magic == 0) // alignment
    {
      System.out.println(" . rectangle aligned with the axis: " + (Math.abs(p1.getX() - p2.getX()) <= 0.01f || Math.abs(p1.getY() - p2.getY()) <= 0.01f));
    }
    else if (magic == 1) // connection
    {
      nf.setMinimumFractionDigits(2);
      nf.setMaximumFractionDigits(2);

      System.out.println(" . distance between arrow head and circle: " + nf.format(Math.abs(distance(p1, p2) - radius)));
    }
    else if (magic == 2) // congruence
    {
      System.out.println(" . are the two circles aligned vertically: " + (Math.abs(p1.getY() - p2.getY()) <= 0.01f));
    }
  }

} // AdjustingBeautifierProperties
