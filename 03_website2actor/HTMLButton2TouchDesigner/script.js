document.querySelector("#btn").addEventListener("click", function () {
  // fetch("http://localhost:9970", { mode: "no-cors" })
  fetch("http://localhost:9970")
    .then((data) => {
      // Handle the response data here
      console.log("successful");
    })
    .catch((error) => {
      console.error(error);
      console.log("an error: Status code:", error.status);
    });
  console.log("Button clicked!");
});
