<template>
  <div class="quiz" v-if="question">
    <h2 v-html="question.title"></h2>
    <img :src="(answered === null) ? `../static/images/${question.image}.jpg` : `../static/images/${question.imageAnswered}.jpg`" class="image"/>
    <br />
    <strong>Choices:</strong>
    <ol class="choices">
      <li v-for="(choice, index) in question.choices" class="choice">
        <button @click="answer(index)" :class="{correct: index === correct, answered: index === answered}" :disabled="answered !== null">
          {{ abc[index] }}. {{ choice }}
        </button>
      </li>
    </ol>

    <strong>Up to how many choices?</strong>
    <select v-model="chooseFrom" @change="newQuestion">
      <option v-for="num in 7">
         {{ num + 1 }}
      </option>
    </select><br />

    <strong>Difficulty</strong>
    <select v-model="difficulty" @change="newQuestion">
      <option v-for="(quizes, difficulty) in quizTypes">
         {{ difficulty }}
      </option>
    </select><br />

    <strong>Include DLC?</strong>
    <input type="checkbox" v-model="includeDLC" @change="newQuestion" />
  </div>
</template>

<script>
import shrines from '../assets/shrinedata.json';
import _ from 'lodash';
export default {
  name: 'Quiz',
  mounted() {
    this.newQuestion();
  },
  data () {
    return {
      publicPath: process.env.BASE_URL,
      abc: ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'],
      chooseFrom: 4,
      question: null,
      quiz: null,
      difficulty: 'normal',
      includeDLC: false,
      questionTimeout: 2000,
      quizTypes: {
        easy: [
          'guessTheMonk',
          'guessTheTrial',
          'guessTheShrine'
        ],
        normal: [
          'guessTheMonk',
          'guessTheTrial',
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
          'guessTheShrineFromQuest',
          'guessTheShrineFromLandmark',
          'guessTheLandmark',
          'guessTheQuest',
          'guessTheItem'
        ]
      },
      answered: null,
      correct: null
    }
  },
  methods: {
    newQuestion() {
      const quiz = this.quizTypes[this.difficulty][_.random(0, this.quizTypes[this.difficulty].length - 1)];
      this.quiz = quiz;
      this.question = this[quiz]();
    },
    randomShrine(shrines) {
      return shrines[_.random(0, shrines.length - 1)];
    },
    answer(index) {
      this.$emit('answer', this.question.choices[index] === this.question.choices[this.question.answer]);
      this.answered = index;
      this.correct = this.question.choices[index] === this.question.choices[this.question.answer] ? index : this.question.answer;
      setTimeout(() => {
        this.newQuestion();
        this.answered = null;
        this.correct = null;
      }, this.questionTimeout);
    },
    hasImages(shrine, images) {
      if (!shrine.missing_images) {
        return true;
      }
      for (let i = 0; i < images.length; i++) {
        if (shrine.missing_images.indexOf(images[i]) >= 0) {
          return false;
        }
      }
      return true;
    },
    DLC(shrine) {
      if (!this.includeDLC) {
        return shrine.dlc != true
      }
      return true;
    },
    guessTheMonk() {
      const set = _.filter(shrines, o => {
        return  o.trial.indexOf(o.monk) === -1 &&
                o.trial.indexOf('Test of Strength') === -1 &&
                this.hasImages(o, ['internal', 'title']) &&
                this.DLC(o)
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const merged = {
        'Central': 'Ridgeland',
        'Gerudo': 'Wasteland',
        'Lake': 'Faron',
        'Woodland': 'Hebra',
        'Hateno': 'Dueling Peaks'
      }

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.id !== shrine.id
          }
          if (merged[shrine.region]) {
            return (o.region === shrine.region || o.region === merged[shrine.region]) &&
                o.trial.indexOf(o.monk) === -1 &&
                o.trial.indexOf('Test of Strength') === -1 &&
                this.DLC(o) &&
                o.id !== shrine.id;
          }
          return (o.region === shrine.region) &&
                 o.trial.indexOf(o.monk) === -1 &&
                 o.trial.indexOf('Test of Strength') === -1 &&
                 this.DLC(o) &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> o.monk),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-internal`,
        imageAnswered: `${shrine.id}-title`,
        title: `The trial <strong>${shrine.trial}</strong> is in which shrine?`
      }
    },
    guessTheTrial() {
      const set = _.filter(shrines, o => {
        return o.trial.indexOf(o.monk) === -1 &&
               this.hasImages(o, ['internal', 'title']) &&
               this.DLC(o)
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const merged = {
        'Akkala': 'Lanayru',
        'Woodland': 'Hebra'
      }

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.id !== shrine.id
          }
          if (merged[shrine.region]) {
            return (o.region === shrine.region ||
                   o.region === merged[shrine.region]) &&
                   o.trial.indexOf(o.monk) === -1 &&
                   this.DLC(o) &&
                   o.id !== shrine.id;
          }
          return (o.region === shrine.region) &&
                 o.trial.indexOf(o.monk) === -1 &&
                 this.DLC(o) &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> o.trial),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-internal`,
        imageAnswered: `${shrine.id}-title`,
        title: `The shrine <strong>${shrine.monk}</strong> has which trial for you?`
      }
    },
    guessTheShrine() {
      const set = _.filter(shrines, o => {
        return this.hasImages(o, ['external', 'title']) &&
        this.DLC(o)
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.id !== shrine.id
          }
          return (o.region === shrine.region) &&
                 this.DLC(o) &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> `${o.monk}: ${o.trial}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-external`,
        imageAnswered: `${shrine.id}-title`,
        title: `This shrine is in the <strong>${shrine.region}</strong> region`
      }
    },
    guessTheShrineFromQuest() {
      const set = _.filter(shrines, o => {
        return o.quest &&
               this.hasImages(o, ['quest', 'title'])
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          return o.quest &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> `${o.monk}: ${o.trial}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-quest`,
        imageAnswered: `${shrine.id}-title`,
        title: `Completing the <strong>${shrine.quest}</strong> quest reveals which shrine?`
      }
    },
    guessTheShrineFromLandmark() {
      const set = _.filter(shrines, o => {
        return o.landmark  &&
               this.hasImages(o, ['external', 'title'])
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          return o.landmark &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> `${o.monk}: ${o.trial}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-external`,
        imageAnswered: `${shrine.id}-title`,
        title: `<strong>${shrine.landmark}</strong> is home to what shrine?`
      }
    },
    guessTheLandmark() {
      const set = _.filter(shrines, o => {
        return o.landmark &&
               this.hasImages(o, ['external', 'title'])
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          return o.landmark &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> `${o.landmark}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-title`,
        imageAnswered: `${shrine.id}-external`,
        title: `<strong>${shrine.monk}: ${shrine.trial}</strong> is near what landmark?`
      }
    },
    guessTheQuest() {
      const set = _.filter(shrines, o => {
        return o.quest &&
               this.hasImages(o, ['title', 'quest'])
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          return o.quest &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> `${o.quest}`),
        answer: _.indexOf(choices, shrine),
        image: `${shrine.id}-title`,
        imageAnswered: `${shrine.id}-quest`,
        title: `<strong>${shrine.monk}: ${shrine.trial}</strong> is the shrine for which quest?`
      }
    },
    guessTheItem() {
      const set = _.filter(shrines, o => {
        return o.main_item &&
               this.hasImages(o, ['external', 'title']) &&
               this.DLC(o)
      });

      if (set.length < 1) this.newQuestion();
      const shrine = this.randomShrine(set);

      const choices = _.shuffle(_.concat(shrine, _.slice(_.shuffle(_.filter(shrines,
        (o)=> {
          if (shrine.dlc) {
            return o.dlc === true &&
                   o.id !== shrine.id
          }
          return this.DLC(o) &&
                 o.id !== shrine.id
        }
      )), 0, this.chooseFrom - 1)));

      return {
        choices: _.map(choices, o=> `${o.main_item}`),
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
h1, h2 {
  font-weight: normal;
}
.quiz {
  margin: 0 auto;
  max-width: 800px;
}
.choices {
  display: flex;
  flex-wrap: wrap;
  margin: 0 auto;
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
  height: 5em;
}
.choice button:disabled {
  cursor: default;
}
.choice button:hover {
  opacity: 1;
}
.choice button.answered {
  color: #4b2c25;
  background-color: #d0a298;
  opacity: 1;
}
.choice button.correct,
.choice button.correct.answered {
  color: #2b472b;
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
