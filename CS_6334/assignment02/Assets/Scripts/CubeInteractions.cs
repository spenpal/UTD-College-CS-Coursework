using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class CubeInteractions : MonoBehaviour
{ 
    
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if(GetComponent<Outline>().enabled && Input.GetButton("js1"))
        {
            switch (gameObject.name)
            {
                case "Cube1":
                    Cube1();
                    break;
                case "Cube2":
                    Cube2();
                    break;
                case "Cube3":
                    Cube3();
                    break;
                default:
                    break;
            }
        }
    }

    public void Cube1()
    {
        Vector2 move = new Vector2(-5, 0) * Time.deltaTime;
        transform.Translate(move, Space.World);
    }

    public void Cube2()
    {
        Vector2 rotate = new Vector2(0, 1) * 100f * Time.deltaTime;
        transform.Rotate(rotate, Space.World);
    }

    public void Cube3()
    {
        Color red = new Color(0.934f, 0.305f, 0.084f, 1.000f);
        if (GetComponent<Renderer>().material.color == red)
        {
            GetComponent<Renderer>().material.color = Color.blue;
        }
        else
        {
            GetComponent<Renderer>().material.color = red;
        }

        System.Threading.Thread.Sleep(99);
    }
    
    // Enable outline if pointer touches cube
    public void PointerEnter()
    {
        GetComponent<Outline>().enabled = true;
    }

    // Disable outline if pointer does not touch cube
    public void PointerExit()
    {
        GetComponent<Outline>().enabled = false;
    }
}
