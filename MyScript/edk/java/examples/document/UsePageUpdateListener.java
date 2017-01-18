package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.Layout;
import com.myscript.document.Page;
import com.myscript.document.PageSelection;
import com.myscript.document.PageUpdateListener;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.geometry.Extent;
import common.CFG;

/**
 * The <code>UsePageUpdateListener</code> example code shows you how to handle
 * page updates on a document.
 */
public class UsePageUpdateListener
{
  /**
   * The temporary directory used in this example.
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  
  /**
   * The document file opened to load its ink. (Note that you can rename any
   * notes document as a zip file to watch inside it.)
   */
  private static final String DOCUMENT_FILE = CFG.DATA_DIR + "/example4.notes";

  /**
   * The PageUpdateListener class used to demonstrate the use of callbacks.
   */
  private static final class MyPageUpdateListener extends PageUpdateListener
  {

    protected MyPageUpdateListener(Engine engine) throws NullPointerException
    {
      super(engine);
    }

    protected void added(Page page)
    {
      System.out.println(" . PageUpdateListener ==> onAdd");
    }

    protected void removed(Page page)
    {
      System.out.println(" . PageUpdateListener ==> onRemove");
    }

    protected void onLayoutUpdate(Page page, int layerIndex, Extent extent)
    {
      System.out.println(" . PageUpdateListener ==> onUpdate:");
      System.out.println("     Layer = " + layerIndex);
      System.out.println("     Extent = { \"xmin\":" + extent.xmin + ", \"ymin\":" + extent.ymin + ", \"xmax\":" + extent.xmax + ", \"ymax\":" + extent.ymax + " }");
    }

  };

  /**
   * Main entry point for the <code>UsePageUpdateListener</code> example
   * program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of PageUpdateListener class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of PageUpdateListener class");
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

    System.out.println("3) getting the number of Pages in the Document");
    final int pageCount = document.getPageCount();
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("4) retrieving the first Page of the Document");
    final Page page = document.getPageAt(0);
    System.out.println(" . Page retrieved successfully");
    System.out.println();

    System.out.println("5) retrieving the Layout of the Page");
    final Layout[] layoutHandler = new Layout[1];
    page.getLayout(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to load page layout");
        layoutHandler[0] = (Layout)value.acquire();
      }
    });
    page.sync(); // wait for getLayout() to complete
    final Layout layout = layoutHandler[0];
    System.out.println(" . Layout retrieved successfully");
    final int layerCount = layout.getLayerCount();
    System.out.println(" . the Layout contains " + layerCount + " layer(s)");
    System.out.println();

    System.out.println("6) creating a user object implementing the IPageListener interface");
    final MyPageUpdateListener listener = new MyPageUpdateListener(engine);
    System.out.println(" . page listener created successfully");
    System.out.println();

    System.out.println("7) attaching the listener to the Page");
    page.addListener(listener);
    System.out.println(" . listener attached successfully");
    System.out.println();

    System.out.println("8) creating a selection on the Page");
    PageSelection selection = PageSelection.create(page);
    System.out.println(" . selection created successfully");
    System.out.println();

    System.out.println("9) selecting the whole Page");
    selection.selectAll();
    System.out.println(" . selection performed successfully");
    System.out.println();

    System.out.println("10) erasing the selection from the Page");
    page.erase(selection);
    System.out.println(" . erase operation done successfully");
    System.out.println();

    System.out.println("11) detaching the listener from the Page");
    page.removeListener(listener);
    System.out.println(" . listener detached successfully");
    System.out.println();

    System.out.println("12) destroying the listener");
    listener.dispose();
    System.out.println(" . listener destroyed successfully");
    System.out.println();

    System.out.println("13) cleaning-up: releasing all created objects and destroying the Engine");
    selection.dispose();
    layout.dispose();
    page.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to listen to the page updates on a document");
  }

} // PageUpdateListener
