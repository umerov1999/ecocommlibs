using System;

namespace certificates
{
  /// <summary>
  /// The <c>Certificate</c> class provides the bytes of the user
  /// certificate used to grant the access to the MyScript technologies.
  /// </summary>
  public sealed class MyCertificate
  {
    /// <summary>
    /// Read only property that returns the bytes of the user certificate.
    /// </summary>
    /// 
    /// <value>
    /// The bytes of the user certificate.
    /// </value>
    public static sbyte[] Bytes
    {
      get
      {
        System.Console.Error.WriteLine("/!\\ please configure your certificate before launching the example programs");
        System.Environment.Exit(-1);
        return null;                                
      }
    }
    
  } // MyCertificate
  
} // namespace certificates
