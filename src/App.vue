<template>
  <div id="app">
    <h1>Learn The Shrines</h1>

    <div v-if="answered > 0">
      <strong>Correct Answers:</strong>
      {{ score }} / {{ answered }} ({{ scorePercent }}%)
    </div>

    <Quiz @answer="tallyScore"/>

  </div>
</template>

<script>
import Quiz from './components/Quiz'

export default {
  name: 'App',
  computed: {
    scorePercent() {
      return Math.round((this.score / this.answered) * 100) || 0;
    }
  },
  components: {
    Quiz
  },
  data () {
    return {
      alert: null,
      correct: null,
      answered: 0,
      score: 0
    }
  },
  methods: {
    tallyScore(correct) {
      this.answered++;
      this.correct = false;
      if (correct) {
        this.score++;
        this.correct = true;
      }
    }
  }
}
</script>

<style>
*,
*:before,
*:after {
  box-sizing: border-box;
}

html {
  height: 100%;
}

body {
  display: flex;
  margin: 0;
  min-height: 100%;
  background: repeat 0 0 / 50% url('./assets/bg.jpg') #060903;
  align-items: center;
}

#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  padding: 50px 10px;
  width: 100%;
  text-align: center;
  color: #fff;
}
</style>
