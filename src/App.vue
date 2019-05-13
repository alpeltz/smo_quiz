<template>
  <div id="app">
    <h1>Learn The Shrines</h1>

    <div v-if="answered > 0" class="score" :class="{perfect: score === answered}">
      <strong>Correct Answers:</strong>
      <span>
        {{ score }} / {{ answered }} ({{ getPercent(score, answered) }}%)
      </span>

      <a href="#show-more" @click="toggleMore" v-if="score > 0">{{ showMore ? 'Less Info' : 'More Info' }}</a>
    </div>

    <div v-if="showMore" class="score-table">
      <ul>
        <li v-for="(typeScore, type) in scoreByType" :key="type">
          <dl>
            <dt>{{ quiz[type] }}</dt>
            <dd><span class="bar" :style="{width: `${getPercent(typeScore, max(scoreByType))}%`}">&nbsp;</span><span class="percent">{{ getPercent(typeScore, score) }}%</span></dd>
          </dl>
        </li>
      </ul>
      <button @click="resetScore">Reset</button>
    </div>

    <Quiz @answer="tallyScore" @reset="resetScore" />

  </div>
</template>

<script>
import Quiz from './components/Quiz'
import _ from 'lodash'

export default {
  name: 'App',
  components: {
    Quiz
  },
  data () {
    return {
      alert: null,
      correct: null,
      showMore: false,
      answered: 0,
      score: 0,
      scoreByType: {},
      quiz: {
        guessTheMonk: 'Guess Monk by Trial',
        guessTheTrial: 'Guess Trial by Monk',
        guessTheShrine: 'Guess Shrine by Region',
        guessTheShrineFromQuest: 'Guess Shrine by Quest',
        guessTheShrineFromLandmark: 'Guess Shrine by Landmark',
        guessTheLandmark: 'Guess Landmark',
        guessTheQuest: 'Guess Quest',
        guessTheItem: 'Guess Item'
      }
    }
  },
  methods: {
    tallyScore (correct, type) {
      this.answered++
      this.correct = false
      if (correct) {
        this.score++
        if (this.scoreByType[type]) {
          this.scoreByType[type]++
        } else {
          this.scoreByType[type] = 1
        }
        this.correct = true
      }
    },
    resetScore () {
      if (this.answered > 0 && window.confirm('This will permanently clear your score.')) {
        this.showMore = false
        this.answered = 0
        this.score = 0
        this.scoreByType = {}
      }
    },
    toggleMore () {
      this.showMore = !this.showMore
    },
    getPercent (score, from) {
      return Math.round((score / from) * 100) || 0
    },
    max (obj) {
      return _.max(Object.values(obj))
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

.score-table {
  margin: 1em auto 0;
  max-width: 600px;
}

.score-table dl,
.score-table dt,
.score-table dd,
.score-table ul,
.score-table li {
  margin: 0;
  padding: 0;
}

.score-table ul {
  padding-right: 30px;
  list-style: none;
}

.score-table dl {
  padding-top: 2px;
}

.score-table dt {
  clear: left;
  float: left;
  padding-right: 1em;
  width: 50%;
  text-align: left;
}

.score-table dd {
  position: relative;
  float: right;
  width: 50%;
}

.score-table li::after {
  clear: both;
  display: table;
  content: '';
}

.score-table .bar {
  display: block;
  background: rgba(255,255,255,.8);
}

.score-table .percent {
  position: absolute;
  padding-left: 5px;
  left: 100%;
  top: 50%;
  transform: translateY(-50%);
}

a {
  color: inherit;
}

.score.perfect {
  font-weight: bold;
  color: #98d098;
}

.score.perfect a {
  font-weight: normal;
  color: #fff;
}
</style>
