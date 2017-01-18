package engine;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.MemoryStatistics;

/**
 * The <code>CreateDisposeEngine</code> example code shows you the basic steps
 * to create and dispose of a MyScript <code>Engine</code>, properly initialized
 * with a certificate that identifies you as a legitimate MyScript user.
 */
public class CreateDisposeEngine
{
  /**
   * Main entry point for the <code>CreateDisposeEngine</code> example program.
   * 
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args)
  {
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();
    
    System.out.println("2) gathering native engine memory statistics:");
    final MemoryStatistics memoryStatistics = engine.getMemoryStatistics();
    System.out.println("   . current allocation count = " + memoryStatistics.getCurrentAllocationCount());
    System.out.println("   . total allocation count = " + memoryStatistics.getTotalAllocationCount());
    System.out.println("   . current memory usage in bytes = " + memoryStatistics.getCurrentMemoryUsage());
    System.out.println("   . maximum memory usage in bytes = " + memoryStatistics.getMaximumMemoryUsage());
    System.out.println();

    System.out.println("3) do not forget the 1st rule: you create it, you dispose of it!");
    System.out.println(" . is the Engine disposed of = " + engine.isDisposed());
    System.out.println(" . disposing of the Engine");
    engine.dispose();
    System.out.println(" . is the Engine disposed of = " + engine.isDisposed());
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to create and dispose of an Engine");
  }
  
} // CreateDisposeEngine
