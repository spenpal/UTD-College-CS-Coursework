import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

class multiplyTwoTest {

    @Test
    void multiTrue()
    {
        multiplyTwo test = new multiplyTwo();
        assertEquals(54, test.multi(6, 9)); // Valid Test Case
    }

    @Test
    void multiFalse()
    {
        multiplyTwo test = new multiplyTwo();
        assertEquals(69, test.multi(6, 9)); // Invalid Test Case
    }

}