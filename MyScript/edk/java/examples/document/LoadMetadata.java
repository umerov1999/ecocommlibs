package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.Page;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.json.Json;
import com.myscript.json.JsonObjectIterator;
import com.myscript.json.JsonType;
import common.CFG;

/**
 * The <code>LoadMetadata</code> example code shows you how to load metadata
 * from a document.
 */
public class LoadMetadata
{
  /**
   * The temporary directory used in this example.
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  
  /**
   * The document file opened to load its metadata. (Note that you can rename
   * any notes document as a zip file to watch inside it.)
   */
  private static final String DOCUMENT_FILE = CFG.DATA_DIR + "/example1.notes";

  /**
   * Main entry point for the <code>LoadMetadata</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of LoadMetadata class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of LoadMetadata class");
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

    System.out.println("3) getting the metadata of the Document");
    final Json[] metadataHolder = new Json[1];
    document.getMetadata(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject result)
      {
        if (!success)
          throw new RuntimeException("failed to retrieve document metadata");
        metadataHolder[0] = (Json)result.acquire();
      }
    });
    document.sync(); // wait for getMetadata() to complete
    final Json documentMetadata = metadataHolder[0];
    System.out.println(" . Document's metadata loaded successfully");
    System.out.println();

    System.out.println("4) getting the version of the Document from its metadata");
    final Json formatVersion = documentMetadata.getObjectEntryValue("format-version");
    System.out.println(" . Document's version = " + formatVersion.getStringValue());
    formatVersion.dispose();
    documentMetadata.dispose();
    System.out.println();

    System.out.println("5) getting the number of pages of the Document");
    final int pageCount = document.getPageCount();
    System.out.println(" . the Document contains " + pageCount + " pages");
    System.out.println();

    System.out.println("6) getting all the pages' metadata");
    for (int pageId = 0; pageId < pageCount; ++pageId)
    {
      final Page page = document.getPageAt(pageId);
      System.out.println(" . page " + pageId);
      page.getMetadata(new IAsyncCommandHandler()
      {
        public void onCompletion(AsyncCommand cmd, boolean success,
                                 EngineObject result)
        {
          if (!success)
            throw new RuntimeException("failed to retrieve page metadata");
          metadataHolder[0] = (Json)result.acquire();
        }
      });
      page.sync(); // wait for getMetadata() to complete
      final Json pageMetadata = metadataHolder[0];
      System.out.println("   . metadata loaded successfully");

      final JsonObjectIterator iterator = pageMetadata.getObjectEntries();
      while (!iterator.isAtEnd())
      {
        final String key = iterator.getKey();
        final Json value = iterator.getValue();
        if (value.getType() == JsonType.STRING)
        {
          System.out.println("    . " + key + " = " + value.getStringValue());
        }
        value.dispose();
        iterator.next();
      }

      iterator.dispose();
      pageMetadata.dispose();
      page.dispose();
    }
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("7) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to load metadata from a document");
  }

} // LoadMetadata
