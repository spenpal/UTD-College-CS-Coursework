using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Character : MonoBehaviour
{
   public float speed = 100.0f;
   private Vector3 moveDirection = Vector3.zero;
   private CharacterController controller;
   public GameObject firstCube, secondCube, thirdCube;  

    void Start()
    {
        controller=GetComponent<CharacterController>();

        // Disable outlines initally
        firstCube = GameObject.Find("Cube1");
        secondCube = GameObject.Find("Cube2");
        thirdCube = GameObject.Find("Cube3");

        firstCube.GetComponent<Outline>().enabled = false;
        secondCube.GetComponent<Outline>().enabled = false;
        thirdCube.GetComponent<Outline>().enabled = false;
    }

    // Update is called once per frame
    void Update()
    {
        moveDirection=new Vector3(Input.GetAxis("Horizontal"),0,Input.GetAxis("Vertical"));
        moveDirection = Camera.main.transform.TransformDirection(moveDirection);
        moveDirection *= speed;
        moveDirection.y = 0.0f;
        controller.Move(moveDirection * Time.deltaTime);
    }
}