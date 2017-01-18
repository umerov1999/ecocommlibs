package shape;

import java.io.File;
import java.io.IOException;
import java.text.NumberFormat;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.shape.ShapeCandidate;
import com.myscript.shape.ShapeDocument;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeModel;
import com.myscript.shape.ShapeRecognized;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.shape.ShapeSegment;
import common.CFG;

/**
 * The <code>SingleShapeRecognition</code> example code shows you how to perform
 * recognition of a single handwritten shape.
 */
public class SingleShapeRecognition
{
  /**
   * Main entry point for the <code>SingleShapeRecognition</code> example
   * program.
   */
  public static final void main(String[] args) throws IOException
  {
    if (args.length == 0)
    {
      args = new String[1];
      args[0] = CFG.DATA_DIR + "/shape.ink";
    }
    
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

    System.out.println("5) processing input files");
    
    for (int i = 0; i < args.length; ++i)
    {
      if (!new File(args[i]).exists())
      {
        System.err.println("missing ink file: " + args[i]);
        System.exit(-1);
      }
      
      System.out.println(" . parsing: " + new File(args[i]).getCanonicalPath());
      // we force the segmentation by issuing startSegment()/endSegment() calls
      document.startSegment();
      parseInput(args[i], document);
      document.endSegment();
      System.out.println("     . ShapeDocument successfully fueled with digital ink");
      System.out.println();
      
      System.out.println(" . running the shape recognition process");
      recognizer.process(document);
      System.out.println("   . shape recognition performed successfully");
      System.out.println();
      
      System.out.println(" . retrieving the recognition result");
      final ShapeSegment segment = document.getSegmentAt(0);
      System.out.println("   . first ShapeSegment retrieved successfully");
      System.out.print("   . retrieving the number of ShapeCandidates in the ShapeSegment: ");
      int count = segment.getCandidateCount();
      System.out.println(count);
      System.out.println("   . iterating over the ShapeRecognized candidates:");
      
      final NumberFormat nf = NumberFormat.getInstance();
      nf.setMinimumFractionDigits(4);
      nf.setMaximumFractionDigits(4);
      for (int j = 0; j < count; ++j)
      {
        final ShapeCandidate candidate = segment.getCandidateAt(j);
  
        if (candidate instanceof ShapeRecognized)
        {
          final ShapeModel model = ((ShapeRecognized)candidate).getModel();
  
          final float rs = ((ShapeRecognized)candidate).getResemblanceScore();
          final float nrs = ((ShapeRecognized)candidate).getNormalizedRecognitionScore();
  
          System.out.println("     . " + model.getLabel() + " (rs:" + nf.format(rs) + " - nrs:" + nf.format(nrs) + ')');
         
          model.dispose();
        }
  
        candidate.dispose();
      }
      System.out.println();
      
      segment.dispose();
      document.clear();
    }
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("6) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform single shape recognition");
  }
  
  private final static void parseInput(final String path, ShapeDocument document)
  {
    final Ink ink = (Ink)EngineObject.load(document.getEngine(), path);
    
    for (int i = 0; i < ink.getStrokeCount(); ++i)
    {
      final InkStroke stroke = ink.getStrokeAt(i);
      
      document.addStroke(stroke.getValuesAsFloat(0), stroke.getValuesAsFloat(1));
      
      stroke.dispose();
    }
  
    ink.dispose();
  }
  
} // SingleShapeRecognition
