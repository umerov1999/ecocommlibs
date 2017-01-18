package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.LayerIterator;
import com.myscript.document.Layout;
import com.myscript.document.Page;
import com.myscript.document.PageSelection;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.geometry.Circle;
import com.myscript.geometry.Rectangle;
import com.myscript.geometry.Transform;
import com.myscript.ink.SelectionModifier;
import common.CFG;

/**
 * The <code>PageEdition/code> example code shows you how to manipulate the
 * content of a page.
 */
public class PageEdition
{
  /**
   * The temporary directory used in this example.
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  
  /**
   * The document file used in this example. (Note that you
   * can rename any notes document as a zip file to watch inside it.)
   */
  private static final String DOCUMENT_FILE       = CFG.DATA_DIR + "/example4.notes";

  /**
   * Main entry point for the <code>PageEdition</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of PageEdition class");
      System.exit(-1);
    }

    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of PageEdition class");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) loading an existing Document");
    final Document document = (Document)EngineObject.load(engine, DOCUMENT_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_FILE).getCanonicalPath() + " loaded successfully");
    System.out.println();

    System.out.println("3) retrieving the first Page of the Document");
    int pageCount = document.getPageCount();
    if (pageCount <= 0)
    {
      System.err.println(" . the Document doesn't contain any page");
      System.exit(-1);
    }
    final Page page = document.getPageAt(0);
    System.out.println(" . Page retrieved successfully");
    System.out.println();

    System.out.println("4) retrieving the Layout of the Page");
    System.out.println(" . sending the Layout request to the Page");
    final Layout[] layoutHolder = new Layout[1];
    page.getLayout(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to get the Layout of the Page");
        else
          layoutHolder[0] = (Layout)(value.acquire());
      }
    });
    System.out.println(" . waiting for the Layout request to complete");
    System.out.println("   . note that sync() should be called only if you need the Layout");
    System.out.println("     immediately and if you have nothing else to do in the meantime");
    page.sync();
    final Layout layout = layoutHolder[0];
    System.out.println(" . Layout loaded successfully");
    System.out.println();

    System.out.println("5) creating a PageSelection");
    final PageSelection selection = PageSelection.create(page);
    System.out.println(" . PageSelection created successfully");
    System.out.println();

    System.out.println("6) selecting a Layer of the Page");
    System.out.println(" . retrieving an iterator over the Layers");
    final String inkLayerName = "Ink";
    LayerIterator layerIterator = layout.getLayer(inkLayerName);
    if (layerIterator.isAtEnd())
    {
      System.err.println(" . the Layout doesn't contain a Layer \"" + inkLayerName + "\"");
      System.exit(-1);
    }
    System.out.println(" . selecting Layer");
    selection.selectLayer(layerIterator, SelectionModifier.SET);
    System.out.println(" . releasing the LayerIterator");
    layerIterator.dispose();
    System.out.println(" . selection performed successfully");
    System.out.println();

    System.out.println("7) selecting a rectangle within the selection");
    final Rectangle rectangle = new Rectangle(32.0f, 0.0f, 300.0f, 500.0f);
    selection.selectRectangle(rectangle, SelectionModifier.INTERSECTION);
    System.out.println(" . rectangle selection performed successfully");
    System.out.println();

    System.out.println("8) erasing the selected portion of the Page");
    page.erase(selection);
    System.out.println(" . selection erased successfully");
    System.out.println();

    System.out.println("9) checking that the Page is marked as modified");
    if (page.isModified() == false)
    {
      System.err.println(" . Page isn't marked as modified successfully");
      System.exit(-1);
    }
    System.out.println(" . Page marked as modified successfully");
    System.out.println();

    System.out.println("10) selecting the closest InkStroke from a given point");
    final Circle circle = new Circle(386.0f, 264.0f, 10.0f);
    selection.hit(circle, SelectionModifier.SET);
    if (selection.getStrokeCount() != 1)
    {
      System.err.println(" . selection wasn't performed successfully");
      System.exit(-1);
    }
    System.out.println(" . selection performed successfully");
    System.out.println();

    Transform transform = new Transform();
    System.out.println("11) creating a rotation transform");
    transform.setIdentity();
    transform.rotate(3.14f, 470.0f, 293.0f);
    System.out.println(" . transform created successfully");
    System.out.println();

    System.out.println("12) applying rotation on the selected InkStroke");
    page.transform(selection, transform);
    System.out.println(" . rotation performed successfully");
    System.out.println();

    System.out.println("13) adding a new Layer to the Layout");
    final int layerCount = layout.getLayerCount();
    final String newLayerName = "newLayer";
    layout.addLayer(newLayerName);
    if (layout.getLayerCount() != (layerCount + 1))
    {
      System.err.println(" . impossible to add a new Layer");
      System.exit(-1);
    }
    System.out.println(" . new Layer added successfully");
    System.out.println();

    System.out.println("14) moving selected content to the new Layer");
    System.out.println(" . retrieving an iterator over the Layers");
    layerIterator = layout.getLayer(newLayerName);
    if (layerIterator.isAtEnd())
    {
      System.err.println(" . the Layout doesn't contain a Layer \"" + newLayerName + "\"");
      System.exit(-1);
    }
    System.out.println(" . moving the selection to the Layer");
    page.moveToLayer(selection, layerIterator);
    System.out.println(" . releasing the LayerIterator");
    layerIterator.dispose();
    System.out.println(" . selection moved successfully");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("15) cleaning-up: disposing of objects and disposing of the Engine");
    selection.dispose();
    layout.dispose();
    page.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to manipulate the content of a page");
  }

} // PageEdition
