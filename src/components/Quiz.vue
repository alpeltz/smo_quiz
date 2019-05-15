<template>
  <div class="quiz" v-if="question">
    <h2 v-html="question.title" v-if="answered === null"></h2>
    <h2 :class="{'alert-correct': answered === correct, 'alert-incorrect': answered !== correct}" v-else v-html="(answered === correct) ? 'Yep! It\'s <strong>'+question.choices[correct]+'</strong>' : 'Sorry, the answer was <strong>'+question.choices[correct]+'</strong>.'"></h2>

    <progress :value="timer" :max="((answered !== correct) ? questionTimeoutIncorrect : questionTimeoutCorrect) - 100"></progress>

    <img :src="(answered === null) ? `../static/images/${question.image}.jpg` : `../static/images/${question.imageAnswered}.jpg`" class="image" :class="{'image-correct': answered === correct && answered !== null, 'image-incorrect': answered !== correct}"/><br />

    <strong>Choices:</strong>
    <ol class="choices">
      <li v-for="(choice, index) in question.choices" class="choice" :key="index">
        <button @click="answer(index)" :class="{correct: index === correct, answered: index === answered}" :disabled="answered !== null">
          {{ abc[index] }}. {{ choice }}
        </button>
      </li>
    </ol>
  </div>
</template>

<script>
import shrines from '../assets/shrinedata.json'
import _ from 'lodash'

function Sound (src) {
  this.sound = document.createElement('audio')
  this.sound.src = src
  this.sound.setAttribute('preload', 'auto')
  this.sound.setAttribute('controls', 'none')
  this.sound.style.display = 'none'
  document.body.appendChild(this.sound)
  this.play = () => this.sound.play()
}

export default {
  name: 'Quiz',
  props: {
    options: {
      type: Object,
      default: () => {
        return {
          soundOn: true,
          includeDLC: false,
          chooseFrom: 4,
          difficulty: 'normal'
        }
      }
    }
  },
  mounted () {
    this.newQuestion()
    document.title = 'Learn the Shrines'
    document.addEventListener('keypress', this.answerKeypress)
  },
  data () {
    return {
      publicPath: process.env.BASE_URL,
      abc: ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'],
      question: null,
      quiz: null,
      questionTimeoutCorrect: 2000,
      questionTimeoutIncorrect: 4000,
      quizTypes: {
        easy: [
          'guessTheTrial',
          'guessTheShrineFromQuest',
          'guessTheShrineFromLandmark',
          'guessTheShrine',
          'guessTheShrine',
          'guessTheShrine'
        ],
        normal: [
          'guessTheMonk',
          'guessTheTrial',
          'guessTheShrine',
          'guessTheShrine',
          'guessTheShrine',
          'guessTheShrineFromQuest',
          'guessTheShrineFromLandmark',
          'guessTheLandmark',
          'guessTheQuest'
        ],
        hard: [
          'guessTheMonk',
          'guessTheTrial',
          'guessTheShrine',
          'guessTheShrine',
          'guessTheShrine',
          'guessTheShrineFromQuest',
          'guessTheShrineFromLandmarkHard',
          'guessTheLandmarkHard',
          'guessTheQuest',
          'guessTheItem'
        ]
      },
      answered: null,
      correct: null,
      previousShrine: null,
      timer: 0,
      correctSound: new Sound('./static/correct.wav'),
      incorrectSound: new Sound('./static/incorrect.wav')
    }
  },
  methods: {
    newQuestion () {
      const quizTypes = this.quizTypes[this.options.difficulty]
      const quiz = quizTypes[_.random(0, quizTypes.length - 1)]
      this.answered = null
      this.correct = null
      this.quiz = quiz
      this.question = this[quiz]()

      this.preloadImage(`../static/images/${this.question.imageAnswered}.jpg`)
    },
    randomShrine (shrines) {
      const vm = this
      return _.filter(shrines, o => {
        return o.id !== vm.previousShrine
      })[_.random(0, shrines.length - 1)]
    },
    preloadImage (image) {
      const img = document.createElement('img')
      img.src = image
    },
    answer (index) {
      if (index > this.options.chooseFrom - 1 || this.answered != null) {
        return
      }

      window.scrollTo(0, 0)

      if (this.options.soundOn) {
        if (index === this.question.answer) {
          this.correctSound.play()
        } else {
          this.incorrectSound.play()
        }
      }
      this.$emit('answer', index === this.question.answer, this.quiz)
      this.answered = index
      this.correct = this.question.answer

      const interval = window.setInterval(() => {
        this.timer += 10
      }, 10)

      setTimeout(() => {
        this.timer = 0
        clearInterval(interval)
        this.newQuestion()
      }, index === this.question.answer ? this.questionTimeoutCorrect : this.questionTimeoutIncorrect)
    },
    resetScore () {
      this.$emit('reset')
    },
    answerKeypress (e) {
      const key = e.keyCode
      switch (key) {
        case 97: // a
          this.answer(0)
          break
        case 65: // A
          this.answer(0)
          break
        case 98: // b
          this.answer(1)
          break
        case 66: // B
          this.answer(1)
          break
        case 99: // c
          this.answer(2)
          break
        case 67: // C
          this.answer(2)
          break
        case 100: // d
          this.answer(3)
          break
        case 68: // D
          this.answer(3)
          break
        case 101: // e
          this.answer(4)
          break
        case 69: // E
          this.answer(4)
          break
        case 102: // f
          this.answer(5)
          break
        case 70: // F
          this.answer(5)
          break
        case 103: // g
          this.answer(6)
          break
        case 71: // G
          this.answer(6)
          break
        case 104: // g
          this.answer(7)
          break
        case 72: // G
          this.answer(7)
          break
        case 105: // h
          this.answer(7)
          break
        case 73: // H
          this.answer(7)
          break
        default:
      }
    },
    hasImages (shrine, images) {
      if (!shrine.missing_images) {
        return true
      }
      for (let i = 0; i < images.length; i++) {
        if (shrine.missing_images.indexOf(images[i]) >= 0) {
          return false
        }
      }
      return true
    },
    DLC (shrine) {
      if (!this.options.includeDLC) {
        return shrine.dlc !== true
      }
      return true
    },
    guessTheMonk () {
      const set = _.filter(shrines, o => {
        return o.trial.indexOf(o.monk) === -1 &&
               (this.options.difficulty !== 'hard' && o.trial.indexOf('Test of Strength') === -1) &&
               this.hasImages(o, ['internal', 'title']) &&
               this.DLC(o)
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const merged = {
        'Central': 'Ridgeland',
        'Gerudo': 'Wasteland',
        'Lake': 'Faron',
        'Woodland': 'Hebra',
        'Hateno': 'Dueling Peaks'
      }

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.id !== shrine.id
          }
          if (merged[shrine.region]) {
            return (o.region === shrine.region || o.region === merged[shrine.region]) && // same region merged
                o.trial.indexOf(o.monk) === -1 && // trial contains monk name (blessings, etc)
                (this.options.difficulty !== 'hard' && o.trial.indexOf('Test of Strength') === -1) && // exclude similar trials
                this.DLC(o) && // is DLC enabled? if so, mix it in
                o.id !== shrine.id // no duplicates
          }
          return (o.region === shrine.region) && // same region
                 o.trial.indexOf(o.monk) === -1 && // trial contains monk name (blessings, etc)
                 (this.options.difficulty !== 'hard' && o.trial.indexOf('Test of Strength') === -1) && // exclude similar trials
                 this.DLC(o) && // is DLC enabled? if so, mix it in
                 o.id !== shrine.id // no duplicates
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => o.monk),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-internal`,
        imageAnswered: `${shrine.id}-title`,
        title: `The trial <strong>${shrine.trial}</strong> is in which shrine?`
      }
    },
    guessTheTrial () {
      const set = _.filter(shrines, o => {
        return o.trial.indexOf(o.monk) === -1 &&
               this.hasImages(o, ['external', 'title']) &&
               this.DLC(o)
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const merged = {
        'Akkala': 'Lanayru',
        'Woodland': 'Hebra'
      }

      const choices = _.shuffle(_.concat(shrine, _.slice(_.uniqBy(_.shuffle(_.filter(shrines,
        o => {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.id !== shrine.id
          }
          if (merged[shrine.region]) {
            return (o.region === shrine.region ||
                   o.region === merged[shrine.region]) && // same region merged
                   o.trial.indexOf(o.monk) === -1 && // trial contains monk name (blessings, etc)
                   this.DLC(o) && // is DLC enabled? if so, mix it in
                   o.trial !== shrine.trial && // for 'twin memories'
                   o.id !== shrine.id
          }
          return (o.region === shrine.region) && // same region
                 o.trial.indexOf(o.monk) === -1 && // trial contains monk name (blessings, etc)
                 this.DLC(o) && // is DLC enabled? if so, mix it in
                 o.trial !== shrine.trial && // for 'twin memories'
                 o.id !== shrine.id
        }
      )), 'trial'), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => o.trial),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-external`,
        imageAnswered: `${shrine.id}-title`,
        title: `The shrine <strong>${shrine.monk}</strong> has which trial for you?`
      }
    },
    guessTheShrine () {
      const set = _.filter(shrines, o => {
        return this.hasImages(o, ['external', 'title']) &&
               this.DLC(o)
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.id !== shrine.id // only select other DLC shrines
          }
          return (o.region === shrine.region) &&
                 this.DLC(o) && // is DLC enabled? if so, mix it in
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${o.monk}: ${o.trial}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-external`,
        imageAnswered: `${shrine.id}-title`,
        title: `This shrine is in the <strong>${shrine.region}</strong> region`
      }
    },
    guessTheShrineFromQuest () {
      const set = _.filter(shrines, o => {
        return o.quest &&
               this.hasImages(o, ['quest', 'title'])
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          return o.quest &&
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${o.monk}: ${o.trial}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-quest`,
        imageAnswered: `${shrine.id}-title`,
        title: `Completing the <strong>${shrine.quest}</strong> quest reveals which shrine?`
      }
    },
    guessTheShrineFromLandmark () {
      const set = _.filter(shrines, o => {
        return o.landmark &&
               this.hasImages(o, ['external', 'title'])
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          return o.landmark &&
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${o.monk}: ${o.trial}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-external`,
        imageAnswered: `${shrine.id}-title`,
        title: `<strong>${shrine.landmark}</strong> is home to what shrine?`
      }
    },
    guessTheShrineFromLandmarkHard () {
      const set = _.filter(shrines, o => {
        return (o.landmark || o.minor_landmark) &&
               this.hasImages(o, ['external', 'title'])
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          return (o.landmark || o.minor_landmark) &&
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${o.monk}: ${o.trial}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-external`,
        imageAnswered: `${shrine.id}-title`,
        title: `<strong>${(shrine.landmark || shrine.minor_landmark)}</strong> is home to what shrine?`
      }
    },
    guessTheLandmark () {
      const set = _.filter(shrines, o => {
        return o.landmark &&
               this.hasImages(o, ['external', 'title'])
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          return o.landmark &&
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${o.landmark}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-title`,
        imageAnswered: `${shrine.id}-external`,
        title: `<strong>${shrine.monk}: ${shrine.trial}</strong> is near what landmark?`
      }
    },
    guessTheLandmarkHard () {
      const set = _.filter(shrines, o => {
        return (o.landmark || o.minor_landmark) &&
               this.hasImages(o, ['external', 'title'])
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          return (o.landmark || o.minor_landmark) &&
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${(o.landmark || o.minor_landmark)}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-title`,
        imageAnswered: `${shrine.id}-external`,
        title: `<strong>${shrine.monk}: ${shrine.trial}</strong> is near what landmark?`
      }
    },
    guessTheQuest () {
      const set = _.filter(shrines, o => {
        return o.quest &&
               this.hasImages(o, ['title', 'quest'])
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          return o.quest &&
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${o.quest}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-title`,
        imageAnswered: `${shrine.id}-quest`,
        title: `<strong>${shrine.monk}: ${shrine.trial}</strong> is the shrine for which quest?`
      }
    },
    guessTheItem () {
      const set = _.filter(shrines, o => {
        return o.main_item &&
               this.hasImages(o, ['external', 'title']) &&
               this.DLC(o)
      })

      if (set.length < 1) this.newQuestion()
      const shrine = this.randomShrine(set)
      this.previousShrine = shrine.id

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        o => {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.main_item !== shrine.main_item &&
                   o.id !== shrine.id
          }
          return this.DLC(o) &&
                 o.main_item !== shrine.main_item &&
                 o.id !== shrine.id
        }
      )), 0, this.options.chooseFrom - 1)))

      return {
        choices: _.map(choices, o => `${o.main_item}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-external`,
        imageAnswered: `${shrine.id}-title`,
        title: `<strong>${shrine.monk}: ${shrine.trial}</strong> contains which item?`
      }
    }
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1,
h2 {
  font-weight: normal;
}

@media (max-width: 600px) {
  h1,
  h2 {
    font-size: 18px;
  }
}
.quiz {
  margin: 0 auto;
  max-width: 800px;
}
.choices {
  display: flex;
  flex-wrap: wrap;
  margin: 0 auto 50px;
  max-width: 600px;
}
ol {
  margin-right: -10px;
  margin-left: -10px;
  padding: 0;
  list-style-type: none;
}
li {
  display: inline-block;
  padding: 0 10px;
  box-sizing: border-box;
}
a {
  color: #42b983;
}
.choice {
  width: 50%;
}

.choice button,
.choice button:disabled {
  margin-bottom: 1em;
  padding: 1em;
  width: 100%;
  color: #363636;
  background-color: #d4d4d4;
  opacity: .8;
  cursor: pointer;
  font-size: 110%;
  min-height: 5em;
}

@media (max-width: 600px) {
  .choice {
    width: 100%;
    padding: 0;
  }

  .choice button,
  .choice button:disabled {
    margin-bottom: 2px;
    padding: .5em 1em;
    min-height: 3.5em;
    font-size: 100%;
  }
}

.choice button:disabled {
  cursor: default;
}
.choice button:hover {
  opacity: 1;
}
.choice button.answered {
  color: #4b2c25;
  border-color: #4b2c25;
  background-color: #d0a298;
  opacity: 1;
}
.choice button.correct,
.choice button.correct.answered {
  color: #2b472b;
  border-color: #2b472b;
  background-color: #98d098;
  opacity: 1;
}
.image {
  margin: 0 auto 1em;
  max-width: 100%;
  height: auto;
  padding: 5px;
  border-radius: 5px;
  background: rgba(255,255,255,0.5);
}

.image-correct {
  background-color: #98d098;
}
.alert-correct {
  color: #98d098;
}

.image-incorrect {
  background-color: #d0a298;
}
.alert-incorrect {
  color: #d0a298;
}

progress[value] {
  margin-bottom: 1em;
  max-width: 800px;
  width: 100%;
  height: 2px;
  appearance: none;
}

progress[value]::-webkit-progress-bar {
  background-color: transparent;
}

progress[value]::-webkit-progress-value {
  background-color: #fff;
}

</style>
<!--

Type of quizes:

1. Guess the monk:
   - choices: shrines in the same region
   - image: trial name + internal shrine image (without title)
   - exclude: shrines with monk's name in trial
2. Guess the trial:
   - choices: shrines in the same region
   - image: monk name + internal shrine image (without title)
3. Guess the monk & trial:
   - choices: shrines in the same region
   - image: external shrine image
4. Guess the monk & trial (shrine quest):
   - choices: shrines for other shrine quests
   - image: shrine quest assignment (from NPC)
5. Guess the monk & trial (landmark):
   - choices: shrines for other landmarks
   - image: external shrine image
6. Guess the landmark:
   - choices: other landmarks
   - image: internal shrine image (with title)
7. Guess the shrine quest:
   - choices: other shrine quests
   - image: internal shrine image (with title)
8. HARD MODE - Guess the main item:
   - choices: shrines in the same region
   - image: internal shrine image (with title)

Images needed:

1. internal - Internal shrine image (without title)
2. title - Internal shrine image (with title)
3. external - External shrine image
5. quest - Shrine quest assignment (from NPC)

-->
