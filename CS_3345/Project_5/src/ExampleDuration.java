import java.time.Duration;

public class ExampleDuration
{
    /**
     * If you are curious about the different ways of timing how long an operation
     * takes in Java (and why <code>System.nanoTime()</code> makes the most sense),
     * then take a look at <a href="https://stackoverflow.com/q/37067929/5029307">
     * this StackOverflow question</a>.
     * <p>
     * Note that you should <b>never</b> compare two "nano times" directly (i.e.
     * don't do <code>timeA \< timeB</code>).  Read the documentation on {@link
     * java.time.Duration #nanoTime()} if you're curious why.
     *
     * @return How long it took to perform the operation
     *
     * @ImplSpec You should perform any "set up" that should not be timed before
     * getting the start time.  Likewise, perform any "tear down" that should not be
     * timed after getting the finish time.
     *
     * @ImplNote Return a <code>Duration</code> object rather than a raw <code>
     * long</code> value so that the client code does not accidentally confuse
     * a nano time with a milli time (which are both represented using a <code>
     * long</code>).
     *
     * @ImplNote Only create a <code>Duration</code> object <i>after</i> the
     * operation has ended -- rather than creation a <code>Duration</code> object
     * for both the start and end time -- in order to avoid accidentally including
     * the time it takes for the JVM to create and register the <code>Duration
     * </code> object in the elapsed operation time.
     */
    public static Duration timedOperation()
    {
        long startTime = System.nanoTime();

        // TODO perform the actual operation(s) that you want to time

        long finishTime = System.nanoTime();
        Duration elapsedTime = Duration.ofNanos(finishTime - startTime);

        return elapsedTime;
    }
}
