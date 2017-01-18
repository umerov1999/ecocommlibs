package shape;

import java.text.NumberFormat;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.shape.ShapeCandidate;
import com.myscript.shape.ShapeDocument;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeModel;
import com.myscript.shape.ShapeRecognized;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.shape.ShapeSegment;
import common.CFG;

/**
 * The <code>AdjustingModelsDetectionSensitivity</code> example code shows you
 * how to adjust models' detection sensitivity.
 */
public class AdjustingModelsDetectionSensitivity
{
  /**
   * Main entry point for the <code>AdjustingModelsDetectionSensitivity</code>
   * example program.
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
    ShapeDocument document = ShapeDocument.create(engine);
    System.out.println(" . ShapeDocument created successfully");
    System.out.println();

    System.out.println("5) fueling the ShapeDocument with digital ink");
    System.out.println(" . adding almost a rectangle");
    System.out.println("     ,----------.");
    System.out.println("    /            \\");
    System.out.println("   |              |");
    System.out.println("   |              |");
    System.out.println("    \\            /");
    System.out.println("     '----------'");
    // as we are doing single shape recognition,
    // we force the segmentation by issuing startSegment()/endSegment() calls
    document.startSegment();
    for (int i = 0; i < Ink.ALMOST_RECTANGLE.length; ++i)
    {
      float[] xy = Ink.ALMOST_RECTANGLE[i];
      
      document.addStroke(xy, 0, 2, xy, 1, 2, xy.length / 2);
    }
    document.endSegment();
    System.out.println(" . ShapeDocument successfully fueled with digital ink");
    System.out.println();

    System.out.println("6) taking a snapshot of the ShapeDocument before recognition");
    ShapeDocument snapshot = null;
    try
    {
      snapshot = (ShapeDocument)document.clone();
    }
    catch(final CloneNotSupportedException e)
    {
      // Java checked exception are that great!
    }
    System.out.println(" . snapshot taken successfully");
    System.out.println();
    
    System.out.println("7) running the shape recognition process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    System.out.println();
    
    System.out.println("8) retrieving the recognition result");
    printResult(document);
    System.out.println();

    System.out.println("9) disabling ellipse and circle ShapeModels");
    final int count = recognizer.getModelCount();
    for (int i = 0; i < count; ++i)
    {
      final ShapeModel model = recognizer.getModelAt(i);
      
      final String label = model.getLabel();
      if (label.equals("ellipse") || label.equals("circle"))
        recognizer.setModelDetectionSensitivity(model, 0);
      
      model.dispose();
    }
    System.out.println(" . ellipse and circle ShapeModels disabled successfully");
    System.out.println();

    System.out.println("10) swapping the ShapeDocument with its snapshot before recognition");
    document.dispose();
    document = snapshot;
    System.out.println(" . snapshot swapped successfully");
    System.out.println();
    
    System.out.println("11) running the shape recognition process");
    recognizer.process(document);
    System.out.println(" . shape recognition performed successfully");
    System.out.println();
    
    System.out.println("12) retrieving the recognition result");
    printResult(document);
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("13) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to adjust models detection sensitivity");
  }
  
  private static void printResult(final ShapeDocument document)
  {
    final ShapeSegment segment = document.getSegmentAt(0);
    System.out.println(" . first ShapeSegment retrieved successfully");
    
    System.out.print(" . retrieving the number of ShapeCandidates in the ShapeSegment: ");
    int count = segment.getCandidateCount();
    System.out.println(count);
    System.out.println(" . iterating over the ShapeRecognized candidates:");
    
    final NumberFormat nf = NumberFormat.getInstance();
    nf.setMinimumFractionDigits(4);
    nf.setMaximumFractionDigits(4);
    for (int i = 0; i < count; ++i)
    {
      final ShapeCandidate candidate = segment.getCandidateAt(i);

      if (candidate instanceof ShapeRecognized)
      {
        final ShapeModel model = ((ShapeRecognized)candidate).getModel();

        final float rs = ((ShapeRecognized)candidate).getResemblanceScore();
        final float nrs = ((ShapeRecognized)candidate).getNormalizedRecognitionScore();

        System.out.println("   . " + model.getLabel() + " (rs:" + nf.format(rs) + " - nrs:" + nf.format(nrs) + ')');
       
        model.dispose();
      }

      candidate.dispose();
    }

    segment.dispose();
  }

} // AdjustingModelsDetectionSensitivity
