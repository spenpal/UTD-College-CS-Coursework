﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class Character : MonoBehaviour
{
    public float speed = 100.0f;
    private Vector3 moveDirection = Vector3.zero;
    private CharacterController controller;
    public GameObject gvrEventSystem;
    public GameObject pauseMenuCanvas, resumeButton, quitButton;

    void Awake()
    {
        
    }

    void Start()
    {
        controller=GetComponent<CharacterController>();
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetButtonDown("A"))
            Pause();

        moveDirection = new Vector3(Input.GetAxis("Horizontal"),0,Input.GetAxis("Vertical"));
        moveDirection = Camera.main.transform.TransformDirection(moveDirection);
        moveDirection *= speed;
        moveDirection.y = 0.0f;
        controller.Move(moveDirection * Time.deltaTime);
    }

    public void Pause()
    {
        gvrEventSystem.GetComponent<GvrPointerInputModule>().enabled = false;
        gvrEventSystem.GetComponent<StandaloneInputModule>().enabled = true;

        Time.timeScale = 0;
        pauseMenuCanvas.SetActive(true);

        // Clear selected objects from EventSystem
        EventSystem.current.SetSelectedGameObject(null);

        // Select new object
        EventSystem.current.SetSelectedGameObject(resumeButton);
    }

    public void Resume()
    {
        gvrEventSystem.GetComponent<StandaloneInputModule>().enabled = false;
        gvrEventSystem.GetComponent<GvrPointerInputModule>().enabled = true;

        // Clear selected objects from EventSystem
        EventSystem.current.SetSelectedGameObject(null);
        
        Time.timeScale = 1;
        pauseMenuCanvas.SetActive(false);
    }

    public void Quit()
    {
        pauseMenuCanvas.SetActive(false);
        Application.Quit();
    }
}